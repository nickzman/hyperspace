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


#ifndef IMPCUBEVOLUME_H
#define IMPCUBEVOLUME_H



/*#include <Implicit/impSurface.h>
#include <Implicit/impCubeTables.h>
#include <Implicit/impCrawlPoint.h>*/
#include "impSurface.h"
#include "impCubeTables.h"
#include "impCrawlPoint.h"
#include <vector>
#include <list>

#include <math.h>

typedef struct HyperspaceSaverSettings;

struct cubedata{
	// corner mask which describes how cube is polygonized
	unsigned int mask;
	// position left-lower-far corner (-x, -y, -z)
	float x;
	float y;
	float z;
	// field value at this corner
	float value;
	// edge vertex indices for surfaces
	unsigned int x_vertex_index;
	unsigned int y_vertex_index;
	unsigned int z_vertex_index;
	// done flags
	bool cube_done;
	bool corner_done;
};


// For making a list of cubes to be polygonized.
// The list can be sorted by depth before polygonization in
// the case of transparent surfaces.
class sortableCube{
public:
	unsigned int index;  // index of cube in volume
	float depth;  // distance squared from eyepoint
    
	sortableCube(){}
	sortableCube(unsigned int ind){index = ind;}
	~sortableCube(){}
    
	friend bool operator < (const sortableCube& a, const sortableCube& b){return((a.depth)<(b.depth));}
	friend bool operator > (const sortableCube& a, const sortableCube& b){return((a.depth)>(b.depth));}
	friend bool operator == (const sortableCube& a, const sortableCube& b){return((a.depth)==(b.depth));}
	friend bool operator != (const sortableCube& a, const sortableCube& b){return((a.depth)!=(b.depth));}
};


class impCubeVolume{
public:
	void *contextForFunction;
	float (*function)(float* position, void *context);
private:
	float lbf[3];  // left-bottom-far corner of volume
	float cubewidth;
	unsigned int w, h, l, w_1, h_1, l_1;
	unsigned int triStripPatterns[256][17];
	bool crawlDirections[256][6];
	unsigned int currentVertexIndex;
	std::vector<cubedata> cubes;
	std::vector<unsigned int> cubeIndices;
	std::list<sortableCube> sortableCubes;
	unsigned int currentCubeIndex;
	bool fastnormals;
	float surfacevalue;  // surface's position on gradient
	impSurface* surface;

public:
	impCubeVolume();
	~impCubeVolume();
	// pass dimensions of volume in cubes plus "cubewidth"
	void init(unsigned int width, unsigned int height, unsigned int length, float cw);
	void useFastNormals(bool val){fastnormals = val;}
	void setSurfaceValue(float sv){surfacevalue = sv;}
	float getSurfaceValue(){return surfacevalue;}
	void setSurface(impSurface* s){surface = s;}
	impSurface* getSurface(){return surface;}
	// These routines compute geometry and store it in "surface"
	// Providing an eyepoint indicates that you want to sort the surface
	// so that transparent surfaces will be drawn back-to-front.
	// Providing a list of crawlpoints indicates that you want to use a 
	// surface crawling algorithm.
	// If no crawlpoint list is provided, then every cube is checked, which
	// is slow but thorough (there's no chance of missing a piece of the
	// surface).
	void makeSurface();
	void makeSurface(float eyex, float eyey, float eyez);
	void makeSurface(impCrawlPointVector &cpv);
	void makeSurface(float eyex, float eyey, float eyez, impCrawlPointVector &cpv);

private:
	// x, y, and z define position of cube in this volume
	inline unsigned int calculateCornerMask(unsigned int x, unsigned int y, unsigned int z);
	inline void crawl_nosort(unsigned int x, unsigned int y, unsigned int z);
	inline void crawl_sort(unsigned int x, unsigned int y, unsigned int z);
	inline void uncrawl(unsigned int x, unsigned int y, unsigned int z);
	inline void polygonize(unsigned int index);
	inline void findcornervalues(unsigned int x, unsigned int y, unsigned int z);
	inline void addVertexToSurface(unsigned int axis, unsigned int index);
	inline unsigned int cubeindex(unsigned int i, unsigned int j, unsigned int k)
		{ return (((k * h_1) + j) * w_1) + i; }
};



#endif
