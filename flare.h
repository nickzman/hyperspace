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


#ifndef FLARE_H
#define FLARE_H



/*#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <rsMath/rsMath.h>*/
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "rsMath.h"

#include "Hyperspace.h"

// Externed so flares can be drawn elsewhere
/*extern unsigned int flarelist[4];
extern GLuint flaretex[4];*/


// Generate textures for lens flares
// then applies textures to geometry in display lists
void initFlares(HyperspaceSaverSettings *inSettings);


// Draw a flare at a specified (x,y) location on the screen
// Screen corners are at (0,0) and (1,1)
// alpha = 0.0 for lowest intensity; alpha = 1.0 for highest intensity
void flare(double *pos, float red, float green, float blue, float alpha, HyperspaceSaverSettings *inSettings);



#endif  // FLARE_H
