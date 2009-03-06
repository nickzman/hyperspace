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


//#include <Implicit/impCubeVolume.h>
#include "impCubeVolume.h"
#include "frsqrt.hh"


impCubeVolume::impCubeVolume(){
	int i, j;
	impCubeTables cubeTables;
	for(i=0; i<256; ++i){
		for(j=0; j<17; ++j)
			triStripPatterns[i][j] = cubeTables.triStripPatterns[i][j];
		for(j=0; j<6; ++j)
			crawlDirections[i][j] = cubeTables.crawlDirections[i][j];
	}

	surface = new impSurface;
	init(4, 4, 4, 0.2f);
	surfacevalue = 0.5f;

	fastnormals = false;
}


impCubeVolume::~impCubeVolume(){
	cubes.clear();
	cubeIndices.clear();
	sortableCubes.clear();
}


void impCubeVolume::init(unsigned int width, unsigned int height, unsigned int length, float cw){
	unsigned int i, j, k;
    
	cubes.clear();

	w = width;
	w_1 = width + 1;
	h = height;
	h_1 = height + 1;
	l = length;
	l_1 = length + 1;

	// calculate position of left-bottom-front corner
	cubewidth = cw;
	lbf[0] = -float(w) * cubewidth * 0.5f;
	lbf[1] = -float(h) * cubewidth * 0.5f;
	lbf[2] = -float(l) * cubewidth * 0.5f;

	// allocate cubedata memory and set cube positions
	cubes.resize((width + 1) * (height + 1) * (length + 1));
	for(i=0; i<=width; ++i){
		for(j=0; j<=height; ++j){
			for(k=0; k<=length; ++k){
				const unsigned int index(cubeindex(i,j,k));
				cubes[index].x = lbf[0] + (cubewidth * float(i));
				cubes[index].y = lbf[1] + (cubewidth * float(j));
				cubes[index].z = lbf[2] + (cubewidth * float(k));
				// max possible value indicates that no vertex index has been assigned
				cubes[index].x_vertex_index = 0xFFFFFFFF;
				cubes[index].y_vertex_index = 0xFFFFFFFF;
				cubes[index].z_vertex_index = 0xFFFFFFFF;
				cubes[index].cube_done = false;
				cubes[index].corner_done = false;
			}
		}
	}
}


void impCubeVolume::makeSurface(){
	unsigned int i, j, k;

	// find gradient value at every corner
	for(i=0; i<=w; ++i){
		for(j=0; j<=h; ++j){
			for(k=0; k<=l; ++k){
				const unsigned int ci(cubeindex(i,j,k));
				cubes[ci].value = function(&(cubes[ci].x), contextForFunction);
				cubes[ci].x_vertex_index = 0xFFFFFFFF;
				cubes[ci].y_vertex_index = 0xFFFFFFFF;
				cubes[ci].z_vertex_index = 0xFFFFFFFF;
			}
		}
	}

	// polygonize surface
	currentVertexIndex = 0;
	for(i=0; i<w; ++i){
		for(j=0; j<h; ++j){
			for(k=0; k<l; ++k){
				const unsigned int ci(cubeindex(i,j,k));
				cubes[ci].mask = calculateCornerMask(i, j, k);
				polygonize(ci);
			}
		}
	}

	if(fastnormals)
		surface->calculateNormals();
}


void impCubeVolume::makeSurface(float eyex, float eyey, float eyez){
	unsigned int i, j, k;

	// find gradient value at every corner
	for(i=0; i<=w; ++i){
		for(j=0; j<=h; ++j){
			for(k=0; k<=l; ++k){
				const unsigned int ci(cubeindex(i,j,k));
				cubes[ci].value = function(&(cubes[ci].x), contextForFunction);
				cubes[ci].x_vertex_index = 0xFFFFFFFF;
				cubes[ci].y_vertex_index = 0xFFFFFFFF;
				cubes[ci].z_vertex_index = 0xFFFFFFFF;
			}
		}
	}

	// erase list from last frame
	sortableCubes.clear();

	// collect list of cubes
	sortableCube* sc;
	unsigned int mask;
	for(i=0; i<w; ++i){
		for(j=0; j<h; ++j){
			for(k=0; k<l; ++k){
				const unsigned int ci(cubeindex(i,j,k));
				mask = calculateCornerMask(i, j, k);
				if(mask != 0 && mask != 255){
					cubes[ci].mask = mask;
					sortableCubes.push_back(sortableCube(ci));
					sc = &(sortableCubes.back());
					const float xdist(cubes[ci].x - eyex);
					const float ydist(cubes[ci].y - eyey);
					const float zdist(cubes[ci].z - eyez);
					sc->depth = xdist * xdist + ydist * ydist + zdist * zdist;
				}
			}
		}
	}

	// sort list of cubes
	sortableCubes.sort();

	// polygonize
	currentVertexIndex = 0;
	for(std::list<sortableCube>::iterator c = sortableCubes.begin(); c != sortableCubes.end(); ++c)
		polygonize(c->index);

	if(fastnormals)
		surface->calculateNormals();
}


void impCubeVolume::makeSurface(impCrawlPointVector &cpv){
	int i, j, k;
	bool crawlpointexit;
	unsigned int mask;

	currentCubeIndex = 0;

	// crawl from every crawl point to create the surface
//	std::list<impCrawlPoint>::iterator crawliter = crawlpointlist.begin();
//	while(crawliter != crawlpointlist.end()){
	for(unsigned int cp=0; cp<cpv.size(); ++cp){
		// find cube corresponding to crawl point
		i = int((cpv[cp].position[0] - lbf[0]) / cubewidth);
		if(i < 0)
			i = 0;
		if(i >= int(w))
			i = int(w) - 1;
		j = int((cpv[cp].position[1] - lbf[1]) / cubewidth);
		if(j < 0)
			j = 0;
		if(j >= int(h))
			j = int(h) - 1;
		k = int((cpv[cp].position[2] - lbf[2]) / cubewidth);
		if(k < 0)
			k = 0;
		if(k >= int(l))
			k = int(l) - 1;

		// escape if starting on a finished cube
		crawlpointexit = 0;
		while(!crawlpointexit){
			const unsigned int ci(cubeindex(i,j,k));
			if(cubes[ci].cube_done)
				crawlpointexit = 1;  // escape if starting on a finished cube
			else{  // find index for this cube
				findcornervalues(i, j, k);
				mask = calculateCornerMask(i, j, k);
				// save index for uncrawling
				cubes[ci].mask = mask;
				if(mask == 255)  // escape if outside surface
					crawlpointexit = 1;
				else{
					if(mask == 0){  // this cube is inside volume
						cubes[ci].cube_done = true;
						--i;  // step to an adjacent cube and start over
						if(i < 0)  // escape if you step outside of volume
							crawlpointexit = 1;
					}
					else{
						crawl_nosort(i, j, k);
						crawlpointexit = 1;
					}
				}
			}
		}

//		++crawliter;
	}

	// polygonize
	currentVertexIndex = 0;
	for(unsigned int c=0; c<currentCubeIndex; ++c)
		polygonize(cubeIndices[c]);

	// zero-out done labels
	for(unsigned int n=0; n<w_1*h_1*l_1; ++n){
		cubes[n].x_vertex_index = 0xFFFFFFFF;
		cubes[n].y_vertex_index = 0xFFFFFFFF;
		cubes[n].z_vertex_index = 0xFFFFFFFF;
		cubes[n].cube_done = false;
		cubes[n].corner_done = false;
	}

	// crawl from every crawl point to zero-out done flags
/*	crawliter = crawlpointlist.begin();
	while(crawliter != crawlpointlist.end()){
		// find cube corresponding to crawl point
		i = int((crawliter->position[0] - lbf[0]) / cubewidth);
		if(i < 0)
			i = 0;
		if(i >= int(w))
			i = int(w) - 1;
		j = int((crawliter->position[1] - lbf[1]) / cubewidth);
		if(j < 0)
			j = 0;
		if(j >= int(h))
			j = int(h) - 1;
		k = int((crawliter->position[2] - lbf[2]) / cubewidth);
		if(k < 0)
			k = 0;
		if(k >= int(l))
			k = int(l) - 1;

		// escape if starting on a finished cube
		crawlpointexit = 0;
		while(!crawlpointexit){
			const unsigned int ci(cubeindex(i,j,k));
			if(!(cubes[ci].cube_done))
				crawlpointexit = 1;  // escape if starting on an unused cube
			else{
				index = cubes[ci].index;
				if(index == 0){
					cubes[ci].cube_done = false;
					cubes[ci].corner_done = false;
					cubes[cubeindex(i,j,k+1)].corner_done = false;
					cubes[cubeindex(i,j+1,k)].corner_done = false;
					cubes[cubeindex(i,j+1,k+1)].corner_done = false;
					cubes[cubeindex(i+1,j,k)].corner_done = false;
					cubes[cubeindex(i+1,j,k+1)].corner_done = false;
					cubes[cubeindex(i+1,j+1,k)].corner_done = false;
					cubes[cubeindex(i+1,j+1,k+1)].corner_done = false;
					--i;  // step to an adjacent cube and start over
					if(i < 0)  // escape if you step outside of volume
						crawlpointexit = 1;
				}
				else{
					uncrawl(i, j, k);
					crawlpointexit = 1;
				}
			}
		}

		++crawliter;
	}*/

	if(fastnormals)
		surface->calculateNormals();
}


void impCubeVolume::makeSurface(float eyex, float eyey, float eyez, impCrawlPointVector &cpv){
	int i, j, k;
	bool crawlpointexit;
	unsigned int mask;

	// erase list from last frame
	sortableCubes.clear();

	// crawl from every crawl point to create the surface
//	std::list<impCrawlPoint>::iterator crawliter = crawlpointlist.begin();
//	while(crawliter != crawlpointlist.end()){
	for(unsigned int cp=0; cp<cpv.size(); ++cp){
		// find cube corresponding to crawl point
		i = int((cpv[cp].position[0] - lbf[0]) / cubewidth);
		if(i < 0)
			i = 0;
		if(i >= int(w))
			i = int(w) - 1;
		j = int((cpv[cp].position[1] - lbf[1]) / cubewidth);
		if(j < 0)
			j = 0;
		if(j >= int(h))
			j = int(h) - 1;
		k = int((cpv[cp].position[2] - lbf[2]) / cubewidth);
		if(k < 0)
			k = 0;
		if(k >= int(l))
			k = int(l) - 1;

		// escape if starting on a finished cube
		crawlpointexit = 0;
		while(!crawlpointexit){
			const unsigned int ci(cubeindex(i,j,k));
			if(cubes[ci].cube_done)
				crawlpointexit = 1;  // escape if starting on a finished cube
			else{  // find index for this cube
				findcornervalues(i, j, k);
				mask = calculateCornerMask(i, j, k);
				// save index for uncrawling
				cubes[ci].mask = mask;
				if(mask == 255)  // escape if outside surface
					crawlpointexit = 1;
				else{
					if(mask == 0){  // this cube is inside volume
						cubes[ci].cube_done = true;
						--i;  // step to an adjacent cube and start over
						if(i < 0)  // escape if you step outside of volume
							crawlpointexit = 1;
					}
					else{
						crawl_sort(i, j, k);
						crawlpointexit = 1;
					}
				}
			}
		}

//		++crawliter;
	}

	// find depths of cubes for sorting
	for(std::list<sortableCube>::iterator c = sortableCubes.begin(); c != sortableCubes.end(); ++c){
		const unsigned int ci(c->index);
		const float xdist(cubes[ci].x - eyex);
		const float ydist(cubes[ci].y - eyey);
		const float zdist(cubes[ci].z - eyez);
		c->depth = xdist * xdist + ydist * ydist + zdist * zdist;
	}

	// sort list of cubes
	sortableCubes.sort();

	// polygonize
	currentVertexIndex = 0;
	for(std::list<sortableCube>::iterator c = sortableCubes.begin(); c != sortableCubes.end(); ++c)
		polygonize(c->index);

	// zero-out done labels
	for(unsigned int n=0; n<w_1*h_1*l_1; ++n){
		cubes[n].x_vertex_index = 0xFFFFFFFF;
		cubes[n].y_vertex_index = 0xFFFFFFFF;
		cubes[n].z_vertex_index = 0xFFFFFFFF;
		cubes[n].cube_done = false;
		cubes[n].corner_done = false;
	}

	if(fastnormals)
		surface->calculateNormals();
}


// calculate index into cubetable
inline unsigned int impCubeVolume::calculateCornerMask(unsigned int x, unsigned int y, unsigned int z){
	unsigned int mask(0);

	unsigned int index(cubeindex(x,y,z));
	if(cubes[index].value < surfacevalue)
		mask |= LBF;

	index += w_1 * h_1;  // + z
	if(cubes[index].value < surfacevalue)
		mask |= LBN;

	index += w_1;  // + y
	if(cubes[index].value < surfacevalue)
		mask |= LTN;

	index -= w_1 * h_1;  // - z
	if(cubes[index].value < surfacevalue)
		mask |= LTF;

	index += 1;  // +x
	if(cubes[index].value < surfacevalue)
		mask |= RTF;
	
	index += w_1 * h_1;  // + z
	if(cubes[index].value < surfacevalue)
		mask |= RTN;

	index -= w_1;  // - y
	if(cubes[index].value < surfacevalue)
		mask |= RBN;

	index -= w_1 * h_1;  // - z
	if(cubes[index].value < surfacevalue)
		mask |= RBF;

	return(mask);
}


inline void impCubeVolume::crawl_nosort(unsigned int x, unsigned int y, unsigned int z){
	findcornervalues(x, y, z);
	const unsigned int mask(calculateCornerMask(x, y, z));

	// quit if this cube has been done or does not intersect surface
	const unsigned int ci(cubeindex(x,y,z));
	if(cubes[ci].cube_done)
		return;

	// add this cube to list of crawled cubes, resizing vector if necessary
	const size_t tslsize(cubeIndices.size());
	if(currentCubeIndex == tslsize)
		cubeIndices.resize(tslsize + 1000);
	cubeIndices[currentCubeIndex++] = ci;

	// save index for polygonizing and uncrawling
	cubes[ci].mask = mask;

	// mark this cube as completed
	cubes[ci].cube_done = true;

	// crawl to adjacent cubes
	if(crawlDirections[mask][0] && x > 0)
		crawl_nosort(x-1, y, z);
	if(crawlDirections[mask][1] && x < w-1)
		crawl_nosort(x+1, y, z);
	if(crawlDirections[mask][2] && y > 0)
		crawl_nosort(x, y-1, z);
	if(crawlDirections[mask][3] && y < h-1)
		crawl_nosort(x, y+1, z);
	if(crawlDirections[mask][4] && z > 0)
		crawl_nosort(x, y, z-1);
	if(crawlDirections[mask][5] && z < l-1)
		crawl_nosort(x, y, z+1);
}


inline void impCubeVolume::crawl_sort(unsigned int x, unsigned int y, unsigned int z){
	findcornervalues(x, y, z);
	const int mask(calculateCornerMask(x, y, z));

	// quit if this cube has been done or does not intersect surface
	const unsigned int ci(cubeindex(x,y,z));
	if(cubes[ci].cube_done)
		return;

	// add cube to list
	sortableCubes.push_back(sortableCube(ci));

	// save index for uncrawling
	cubes[ci].mask = mask;

	// mark this cube as completed
	cubes[ci].cube_done = true;

	// crawl to adjacent cubes
	if(crawlDirections[mask][0] && x > 0)
		crawl_sort(x-1, y, z);
	if(crawlDirections[mask][1] && x < w-1)
		crawl_sort(x+1, y, z);
	if(crawlDirections[mask][2] && y > 0)
		crawl_sort(x, y-1, z);
	if(crawlDirections[mask][3] && y < h-1)
		crawl_sort(x, y+1, z);
	if(crawlDirections[mask][4] && z > 0)
		crawl_sort(x, y, z-1);
	if(crawlDirections[mask][5] && z < l-1)
		crawl_sort(x, y, z+1);
}


inline void impCubeVolume::uncrawl(unsigned int x, unsigned int y, unsigned int z){
	if(!(cubes[cubeindex(x,y,z)].cube_done))
		return;

	cubes[cubeindex(x,y,z)].corner_done = false;
	cubes[cubeindex(x,y,z+1)].corner_done = false;
	cubes[cubeindex(x,y+1,z)].corner_done = false;
	cubes[cubeindex(x,y+1,z+1)].corner_done = false;
	cubes[cubeindex(x+1,y,z)].corner_done = false;
	cubes[cubeindex(x+1,y,z+1)].corner_done = false;
	cubes[cubeindex(x+1,y+1,z)].corner_done = false;
	cubes[cubeindex(x+1,y+1,z+1)].corner_done = false;

	cubes[cubeindex(x,y,z)].cube_done = 0;

	cubes[cubeindex(x,y,z)].x_vertex_index = 0xFFFFFFFF;
	cubes[cubeindex(x,y+1,z)].x_vertex_index = 0xFFFFFFFF;
	cubes[cubeindex(x,y,z+1)].x_vertex_index = 0xFFFFFFFF;
	cubes[cubeindex(x,y+1,z+1)].x_vertex_index = 0xFFFFFFFF;
	cubes[cubeindex(x,y,z)].y_vertex_index = 0xFFFFFFFF;
	cubes[cubeindex(x+1,y,z)].y_vertex_index = 0xFFFFFFFF;
	cubes[cubeindex(x,y,z+1)].y_vertex_index = 0xFFFFFFFF;
	cubes[cubeindex(x+1,y,z+1)].y_vertex_index = 0xFFFFFFFF;
	cubes[cubeindex(x,y,z)].z_vertex_index = 0xFFFFFFFF;
	cubes[cubeindex(x+1,y,z)].z_vertex_index = 0xFFFFFFFF;
	cubes[cubeindex(x,y+1,z)].z_vertex_index = 0xFFFFFFFF;
	cubes[cubeindex(x+1,y+1,z)].z_vertex_index = 0xFFFFFFFF;

	// uncrawl adjacent cubes
	const unsigned int mask(cubes[cubeindex(x,y,z)].mask);
	if(crawlDirections[mask][0] && x > 0)
		uncrawl(x-1, y, z);
	if(crawlDirections[mask][1] && x < w-1)
		uncrawl(x+1, y, z);
	if(crawlDirections[mask][2] && y > 0)
		uncrawl(x, y-1, z);
	if(crawlDirections[mask][3] && y < h-1)
		uncrawl(x, y+1, z);
	if(crawlDirections[mask][4] && z > 0)
		uncrawl(x, y, z-1);
	if(crawlDirections[mask][5] && z < l-1)
		uncrawl(x, y, z+1);
}


// polygonize an individual cube
inline void impCubeVolume::polygonize(unsigned int index){
	// find index into cubetable
	const unsigned int mask(cubes[index].mask);

	unsigned int counter = 0;
	unsigned int nedges = triStripPatterns[mask][counter];
	while(nedges != 0){
		surface->addTriStripLength(nedges);
		for(unsigned int i=0; i<nedges; ++i){
			// generate vertex position and normal data
			switch(triStripPatterns[mask][i+counter+1]){
			case 0:
				addVertexToSurface(2, index);
				break;
			case 1:
				addVertexToSurface(1, index);
				break;
			case 2:
				addVertexToSurface(1, index + w_1*h_1);
				break;
			case 3:
				addVertexToSurface(2, index + w_1);
				break;
			case 4:
				addVertexToSurface(0, index);
				break;
			case 5:
				addVertexToSurface(0, index + w_1*h_1);
				break;
			case 6:
				addVertexToSurface(0, index + w_1);
				break;
			case 7:
				addVertexToSurface(0, index + w_1 + w_1*h_1);
				break;
			case 8:
				addVertexToSurface(2, index + 1);
				break;
			case 9:
				addVertexToSurface(1, index + 1);
				break;
			case 10:
				addVertexToSurface(1, index + 1 + w_1*h_1);
				break;
			case 11:
				addVertexToSurface(2, index + 1 + w_1);
				break;
			}
		}
		counter += (nedges + 1);
		nedges = triStripPatterns[mask][counter];
	}
}


// find value at all corners of this cube
inline void impCubeVolume::findcornervalues(unsigned int x, unsigned int y, unsigned int z){
	unsigned int index;

	index = cubeindex(x,y,z);
	if(!cubes[index].corner_done){
		cubes[index].value = function(&(cubes[index].x), contextForFunction);
		cubes[index].corner_done = true;
	}
	++z;
	index = cubeindex(x,y,z);
	if(!cubes[index].corner_done){
		cubes[index].value = function(&(cubes[index].x), contextForFunction);
		cubes[index].corner_done = true;
	}
	++y;
	index = cubeindex(x,y,z);
	if(!cubes[index].corner_done){
		cubes[index].value = function(&(cubes[index].x), contextForFunction);
		cubes[index].corner_done = true;
	}
	--z;
	index = cubeindex(x,y,z);
	if(!cubes[index].corner_done){
		cubes[index].value = function(&(cubes[index].x), contextForFunction);
		cubes[index].corner_done = true;
	}
	++x;
	index = cubeindex(x,y,z);
	if(!cubes[index].corner_done){
		cubes[index].value = function(&(cubes[index].x), contextForFunction);
		cubes[index].corner_done = true;
	}
	++z;
	index = cubeindex(x,y,z);
	if(!cubes[index].corner_done){
		cubes[index].value = function(&(cubes[index].x), contextForFunction);
		cubes[index].corner_done = true;
	}
	--y;
	index = cubeindex(x,y,z);
	if(!cubes[index].corner_done){
		cubes[index].value = function(&(cubes[index].x), contextForFunction);
		cubes[index].corner_done = true;
	}
	--z;
	index = cubeindex(x,y,z);
	if(!cubes[index].corner_done){
		cubes[index].value = function(&(cubes[index].x), contextForFunction);
		cubes[index].corner_done = true;
	}
}


inline void impCubeVolume::addVertexToSurface(unsigned int axis, unsigned int index){
	float data[6];

	// find position of vertex along this edge
	switch(axis){
		case 0:{  // x-axis
			if(cubes[index].x_vertex_index != 0xFFFFFFFF){
				surface->addIndex(cubes[index].x_vertex_index);
				return;
			}
			else{
				surface->addIndex(currentVertexIndex);
				cubes[index].x_vertex_index = currentVertexIndex;
				currentVertexIndex ++;
				data[3] = cubes[index].x
					+ (cubewidth * ((surfacevalue - cubes[index].value)
					/ (cubes[index+1].value - cubes[index].value)));
				data[4] = cubes[index].y;
				data[5] = cubes[index].z;
			}
			break;
		}
		case 1:{  // y-axis
			if(cubes[index].y_vertex_index != 0xFFFFFFFF){
				surface->addIndex(cubes[index].y_vertex_index);
				return;
			}
			else{
				surface->addIndex(currentVertexIndex);
				cubes[index].y_vertex_index = currentVertexIndex;
				currentVertexIndex ++;
				data[3] = cubes[index].x;
				data[4] = cubes[index].y
					+ (cubewidth * ((surfacevalue - cubes[index].value)
					/ (cubes[index+w_1].value - cubes[index].value)));
				data[5] = cubes[index].z;
			}
			break;
		}
		case 2:{  // z-axis
			if(cubes[index].z_vertex_index != 0xFFFFFFFF){
				surface->addIndex(cubes[index].z_vertex_index);
				return;
			}
			else{
				surface->addIndex(currentVertexIndex);
				cubes[index].z_vertex_index = currentVertexIndex;
				currentVertexIndex ++;
				data[3] = cubes[index].x;
				data[4] = cubes[index].y;
				data[5] = cubes[index].z
					+ (cubewidth * ((surfacevalue - cubes[index].value)
					/ (cubes[index+(w_1*h_1)].value - cubes[index].value)));
			}
			break;
		}
	}

	if(!fastnormals){
		// Find normal vector at vertex along this edge
		// First find normal vector origin value
		float* pos = &(data[3]);
		const float val(function(pos, contextForFunction));
		// then find values at slight displacements and subtract
		pos[0] -= 0.01f;
		const float nx(function(pos, contextForFunction) - val);
		pos[0] += 0.01f;
		pos[1] -= 0.01f;
		const float ny(function(pos, contextForFunction) - val);
		pos[1] += 0.01f;
		pos[2] -= 0.01f;
		const float nz(function(pos, contextForFunction) - val);
		pos[2] += 0.01f;
		// then normalize
		const float normalizer(rsqrtf(nx * nx + ny * ny + nz * nz));
		data[0] = nx * normalizer;
		data[1] = ny * normalizer;
		data[2] = nz * normalizer;
	}
	
	// Add this vertex to surface
	surface->addVertex(data);
}
