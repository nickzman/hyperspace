/*
 * Copyright (C) 2001-2005  Terence M. Welsh
 *
 * This file is part of Implicit.
 *
 * Implicit is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * Implicit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef IMPSHAPE_H
#define IMPSHAPE_H


// This is a generic class from which specific elements are
// derived.  An element would be a sphere or torus or anything
// that defines an implicit surface.


//#include <Implicit/impCrawlPoint.h>
#include <math.h>
#include "impCrawlPoint.h"


class impShape{
public:
	float mat[16];
	float invmat[16];
	// Thickness is only necessary in torii and knots, but can be used in other shapes.
	// Thickness is different from scale (which can be applied using an element's
	// matrix) because it only scales the width of appendages and not their relative
	// distances.
	// thicknessSquared is often useful in the value() function, so it is precomputed.
	float thickness, thicknessSquared;

	impShape();
	virtual ~impShape(){};
	void setPosition(float x, float y, float z);
	void setPosition(float* position);
	void setMatrix(float* m);
	void invertMatrix();
	void setThickness(float t){
		thickness = t;
		thicknessSquared = thickness * thickness;
	}
	float getThickness(){return thickness;}
	// returns the value of this element at a given position
	// "position" is an array of 3 floats
	virtual float value(float* position);
	// assigns a center of the element's volume to "position"
	virtual void center(float* position);
	// adds surface crawler start position(s) to given crawlPointVector
	virtual void addCrawlPoint(impCrawlPointVector &cpv);
};




#endif
