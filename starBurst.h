/*
 * Copyright (C) 2005  Terence M. Welsh
 *
 * This file is part of Hyperspace.
 *
 * Hyperspace is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 *
 * Hyperspace is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef STARBURST_H
#define STARBURST_H



#include "stretchedParticle.h"
#include "Hyperspace.h"


#define SB_NUM_STARS 200



class starBurst{
public:
	stretchedParticle** stars;
	// stars only stay active while they're within viewing range
	bool* stars_active;
	float** stars_velocity;
	unsigned int call_list;
	float size;
	float pos[3];

	starBurst();
	~starBurst();
	void restart(float* position);
	void drawStars(HyperspaceSaverSettings *inSettings);
	void draw(HyperspaceSaverSettings *inSettings);  // draw regular
	void draw(float lerp, HyperspaceSaverSettings *inSettings);  // draw with shaders
};



#endif
