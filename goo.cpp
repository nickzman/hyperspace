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


//#include <Hyperspace/goo.h>
#include "goo.h"


/*extern float frameTime, simulationTime;
extern float shiftx, shiftz;
extern float dFov;*/



goo::goo(int res, float rad, float (*func)(float* position, void *context), void *context){
	int i, j;

	volumeSize = 2.0f;
	if(res < 5)
		res = 5;
	resolution = res;
	radius = rad;
	unitSize = volumeSize / float(res);
	arraySize = 2 * int(0.99f + radius / volumeSize);

	// Init implicit surfaces
	volume = new impCubeVolume;
	volume->init(resolution, resolution, resolution, unitSize);
	volume->function = func;
	volume->contextForFunction = context;
	volume->setSurfaceValue(0.4f);
	surface = new impSurface**[arraySize];
	useSurface = new bool*[arraySize];
	for(i=0; i<arraySize; i++){
		surface[i] = new impSurface*[arraySize];
		useSurface[i] = new bool[arraySize];
		for(j=0; j<arraySize; j++){
			surface[i][j] = new impSurface;
			useSurface[i][j] = false;
		}
	}
	volume->useFastNormals(true);
}


goo::~goo(){
	delete[] surface;
	delete[] useSurface;
	delete volume;
}


void goo::update(float x, float z, float heading, float fov, float *shiftx, float *shiftz){
	int i, j;

	float halfFov = 0.5f * fov;

	centerx = unitSize * float(int(0.5f + x / unitSize));
	centerz = unitSize * float(int(0.5f + z / unitSize));

	clip[0][0] = cosf(heading + halfFov);
	clip[0][1] = -sinf(heading + halfFov);
	clip[1][0] = -cosf(heading - halfFov);
	clip[1][1] = sinf(heading - halfFov);
	clip[2][0] = sinf(heading);
	clip[2][1] = -cosf(heading);

	for(i=0; i<arraySize; i++){
		for(j=0; j<arraySize; j++){
			*shiftx = volumeSize * (0.5f + float(i - arraySize / 2));
			*shiftz = volumeSize * (0.5f + float(j - arraySize / 2));
			if(*shiftx * clip[0][0] + *shiftz * clip[0][1] > volumeSize * -1.41421f){
				if(*shiftx * clip[1][0] + *shiftz * clip[1][1] > volumeSize * -1.41421f){
					if(*shiftx * clip[2][0] + *shiftz * clip[2][1] < radius + volumeSize * 1.41421f){
						*shiftx += centerx;
						*shiftz += centerz;
						volume->setSurface(surface[i][j]);
						surface[i][j]->reset();
						volume->makeSurface();
						useSurface[i][j] = true;
					}
				}
			}
		}
	}
}

	
void goo::draw(float *shiftx, float *shiftz){
	int i, j;

	for(i=0; i<arraySize; i++){
		for(j=0; j<arraySize; j++){
			if(useSurface[i][j]){
				*shiftx = centerx + volumeSize * (0.5f + float(i - arraySize / 2));
				*shiftz = centerz + volumeSize * (0.5f + float(j - arraySize / 2));
				glPushMatrix();
				glTranslatef(*shiftx, 0.0f, *shiftz);
				surface[i][j]->draw();
				glPopMatrix();
				useSurface[i][j] = false;
			}
		}
	}
}
