/*
 * Copyright (C) 1999-2005  Terence M. Welsh
 *
 * This file is part of Rgbhsl.
 *
 * Rgbhsl is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * Rgbhsl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */



//#include <Rgbhsl/Rgbhsl.h>
#include "Rgbhsl.h"


void rgb2hsl(float r, float g, float b, float &h, float &s, float &l){
	int huezone = 0;
	float rr, gg, bb;

	// find huezone
	if(r >= g){
		huezone = 0;
		if(b > r)
			huezone = 4;
		else{
			if(b > g)
				huezone = 5;
		}
	}
	else{
		huezone = 1;
		if(b > g)
			huezone = 2;
		else{
			if(b > r)
				huezone = 3;
		}
	}

	// luminosity
	switch(huezone){
	case 0:
	case 5:
		l = r;
		rr = 1.0f;
		gg = g / l;
		bb = b / l;
	break;
	case 1:
	case 2:
		l = g;
		gg = 1.0f;
		rr = r / l;
		bb = b / l;
	break;
	default:
		l = b;
		bb = 1.0f;
		rr = r / l;
		gg = g / l;
	}
	if(l == 0.0){
		h = 0.0;
		s = 1.0;
		return;
	}

	// saturation
	switch(huezone){
	case 0:
	case 1:
		s = 1.0f - b;
		bb = 0.0f;
		rr = 1.0f - ((1.0f - rr) / s);
		gg = 1.0f - ((1.0f - gg) / s);
	break;
	case 2:
	case 3:
		s = 1.0f - r;
		rr = 0.0f;
		gg = 1.0f - ((1.0f - gg) / s);
		bb = 1.0f - ((1.0f - bb) / s);
	break;
	default:
		s = 1.0f - g;
		gg = 0.0f;
		rr = 1.0f - ((1.0f - rr) / s);
		bb = 1.0f - ((1.0f - bb) / s);
	}

	// hue
	switch(huezone){
	case 0:
		h = g / 6.0f;
	break;
	case 1:
		h = ((1.0f - r) / 6.0f) + 0.166667f;
	break;
	case 2:
		h = (b / 6.0f) + 0.333333f;
	break;
	case 3:
		h = ((1.0f - g) / 6.0f) + 0.5f;
	break;
	case 4:
		h = (r / 6.0f) + 0.666667f;
	break;
	default:
		h = ((1.0f - b) / 6.0f) + 0.833333f;
	}
}


void hsl2rgb(float h, float s, float l, float &r, float &g, float &b){
	// hue influence
	if(h < 0.166667){  // full red, some green
		r = 1.0;
		g = h * 6.0f;
		b = 0.0;
	}
	else {
		if(h < 0.5){  // full green
			g = 1.0;
			if(h < 0.333333){  // some red
				r = 1.0f - ((h - 0.166667f) * 6.0f);
				b = 0.0;
			}
			else{  // some blue
				b = (h - 0.333333f) * 6.0f;
				r = 0.0;
			}
		}
		else{
			if(h < 0.833333){  // full blue
				b = 1.0;
				if(h < 0.666667){  // some green
					g = 1.0f - ((h - 0.5f) * 6.0f);
					r = 0.0;
				}
				else{  // some red
					r = (h - 0.666667f) * 6.0f;
					g = 0.0;
				}
			}
			else{  // full red, some blue
				r = 1.0;
				b = 1.0f - ((h - 0.833333f) * 6.0f);
				g = 0.0;
			}
		}
	}

	// saturation influence
	r = 1.0f - (s * (1.0f - r));
	g = 1.0f - (s * (1.0f - g));
	b = 1.0f - (s * (1.0f - b));

	// luminosity influence
	r *= l;
	g *= l;
	b *= l;
}


void hslTween(float h1, float s1, float l1,
					 float h2, float s2, float l2, float tween, int direction,
					 float &outh, float &outs, float &outl){
	// hue
	if(!direction){  // forward around color wheel
		if(h2 >= h1)
			outh = h1 + (tween * (h2 - h1));
		else{
			outh = h1 + (tween * (1.0f - (h1 - h2)));
			if(outh > 1.0)
				outh -= 1.0;
		}
	}
	else{  // backward around color wheel
		if(h1 >= h2)
			outh = h1 - (tween * (h1 - h2));
		else{
			outh = h1 - (tween * (1.0f - (h2 - h1)));
			if(outh < 0.0)
				outh += 1.0;
		}
	}

	// saturation
	outs = s1 + (tween * (s2 - s1));

	// luminosity
	outl = l1 + (tween * (l2 - l1));
}


void rgbTween(float r1, float g1, float b1,
					 float r2, float g2, float b2, float tween, int direction,
					 float &outr, float &outg, float &outb){
	float h1, s1, l1, h2, s2, l2, outh, outs, outl;

	rgb2hsl(r1, g1, b1, h1, s1, l1);
	rgb2hsl(r2, g2, b2, h2, s2, l2);
	hslTween(h1, s1, l1, h2, s2, l2, tween, direction, outh, outs, outl);
	hsl2rgb(outh, outs, outl, outr, outg, outb);
}