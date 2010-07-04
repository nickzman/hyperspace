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


#ifndef GOO_H
#define GOO_H



/*#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <math.h>
#include <rsmath/rsMath.h>
#include <list>
#include <Implicit/impCubeVolume.h>
#include <Implicit/impCrawlPoint.h>*/
#include <OpenGL/gl.h>
#include <math.h>
#include "rsMath.h"
#include <list>
#include "impCubeVolume.h"
#include "impCrawlPoint.h"

typedef struct HyperspaceSaverSettings;


class goo{
public:
	int resolution;
	float radius;
	float unitSize;
	float volumeSize;

	float waveOffset[4];

	float centerx, centerz;
	int arraySize;
	impCubeVolume* volume;
	impSurface*** surface;
	bool** useSurface;

	// normals of planes for culling impSurfaces
	float clip[3][2];

	goo(int res, float rad, float (*func)(float* position, void *context), void *context);
	~goo();
	void update(float x, float z, float heading, float fov, float *shiftx, float *shiftz);
	void draw(float *shiftx, float *shiftz);
};




#endif
