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


//#include <Implicit/impSurface.h>
#include <math.h>
#include <iostream>
#include "impSurface.h"
#include "frsqrt.hh"


impSurface::~impSurface(){
	triStripLengths.resize(0);
	indices.resize(0);
	vertices.resize(0);

	vertex_data_size = sizeof(float) * 6;
}


void impSurface::reset(){
	num_tristrips = 0;
	index_offset = 0;
	vertex_offset = 0;
}


/*void impSurface::addstrip(unsigned char length, std::vector<float*> &data){
	// make more tristrip storage if necessary
	const size_t tslsize(triStripLengths.size());
	if(num_tristrips == tslsize)
		triStripLengths.resize(tslsize + 1000);

	triStripLengths[num_tristrips] = length;

	// make more vertex data storage if necessary
	const size_t datasize(vertices.size());
	if((vertex_offset + (length * 6)) >= datasize)
		vertices.resize(datasize + 1000);

	for(unsigned char i=0; i<length; ++i){
		memcpy(&(vertices[vertex_offset]), data[i], vertex_data_size);
		vertex_offset += 6;
	}

	++num_tristrips;
	//data_index += length * 6;
}*/


void impSurface::addTriStripLength(unsigned char length){
	// make more tristrip storage if necessary
	const size_t tslsize(triStripLengths.size());
	if(num_tristrips == tslsize)
		triStripLengths.resize(tslsize + 1000);

	triStripLengths[num_tristrips++] = length;
}


void impSurface::addIndex(unsigned int index){
	// make more vertex data storage if necessary
	const size_t datasize(indices.size());
	if(index_offset == datasize)
		indices.resize(datasize + 1000);

	indices[index_offset++] = index;
}


void impSurface::addVertex(float* data){
	// make more vertex data storage if necessary
	const size_t datasize(vertices.size());
	if((vertex_offset + 6) >= datasize)
		vertices.resize(datasize + 1000);

	memcpy(&(vertices[vertex_offset]), data, 24);//vertex_data_size);
	vertex_offset += 6;
}


void impSurface::calculateNormals(){
	unsigned int i, j, k=0;
	float vec1[3];
	float vec2[3];
	float norm[3];

	// zero-out normals
	float zeroes[3] = {0.0f, 0.0f, 0.0f};
	for(i=0; i<vertex_offset; i+=6)
		memcpy(&(vertices[i]), zeroes, 12);

	// add normal components from various triangles
	for(i=0; i<num_tristrips; ++i){
		// Calculate normal component at first vertex in tristrip
		subvec(vec1, &(vertices[indices[k+1]*6+3]), &(vertices[indices[k]*6+3]));
		subvec(vec2, &(vertices[indices[k+2]*6+3]), &(vertices[indices[k]*6+3]));
		cross(norm, vec1, vec2);
		// Approximate more weight for smaller triangles
		const float weight(1.0f / (norm[0] * norm[0] + norm[1] * norm[1]
			+ norm[2] * norm[2] + 0.00000000001f));
		norm[0] *= weight;
		norm[1] *= weight;
		norm[2] *= weight;
		// Add this normal component to first 2 vertices
		addvec(&(vertices[indices[k]*6]), &(vertices[indices[k]*6]), norm);
		addvec(&(vertices[indices[k+1]*6]), &(vertices[indices[k+1]*6]), norm);
		for(j=2; j<triStripLengths[i]-1; ++j){
			// Calculate normal component at this vertex
			const unsigned int ind(k+j);
			subvec(vec1, &(vertices[indices[ind+1]*6+3]), &(vertices[indices[ind]*6+3]));
			subvec(vec2, &(vertices[indices[ind-1]*6+3]), &(vertices[indices[ind]*6+3]));
			cross(norm, vec1, vec2);
			// Approximate more weight for smaller triangles
			const float weight(1.0f / (norm[0] * norm[0] + norm[1] * norm[1]
				+ norm[2] * norm[2] + 0.00000000001f));
			// flip normal on even-numbered normals
			const float sign(-1.0f + (j % 2) * 2.0f);
			norm[0] *= weight * sign;
			norm[1] *= weight * sign;
			norm[2] *= weight * sign;
			// Add normal component to this normal
			addvec(&(vertices[indices[ind]*6]), &(vertices[indices[ind]*6]), norm);
		}
		// Use last calculated normal component for the last vertex in this tristrip
		const unsigned int ind(k+triStripLengths[i]-1);
		addvec(&(vertices[indices[ind]*6]), &(vertices[indices[ind]*6]), norm);
		// advance index to next tristrip
		k += triStripLengths[i];
	}

	// normalize the normals
	for(i=0; i<vertex_offset; i+=6){
		const unsigned int ii(i+1);
		const unsigned int iii(i+2);
		const float normalizer(rsqrtf(vertices[i]*vertices[i]+vertices[ii]*vertices[ii]+vertices[iii]*vertices[iii]));
		vertices[i] *= normalizer;
		vertices[ii] *= normalizer;
		vertices[iii] *= normalizer;
	}
}


void impSurface::draw(){
	// draw using regular immediate mode OpenGL
	/*unsigned int i, j, k = 0;
	for(i=0; i<num_tristrips; ++i){
		glBegin(GL_TRIANGLE_STRIP);
			for(j=0; j<triStripLengths[i]; ++j){
				const unsigned int index(indices[k] * 6);
				glNormal3fv(&(vertices[index]));
				glVertex3fv(&(vertices[index+3]));
				++k;
			}
		glEnd();
	}*/

	// draw using vertex arrays
	glInterleavedArrays(GL_N3F_V3F, 0, &(vertices[0]));
	int start_vert = 0;
	for(unsigned int i=0; i<num_tristrips; ++i){
		glDrawElements(GL_TRIANGLE_STRIP, triStripLengths[i], GL_UNSIGNED_INT, &(indices[start_vert]));
		start_vert += triStripLengths[i];
	}
}


inline void impSurface::addvec(float* dest, float* a, float* b){
	dest[0] = a[0] + b[0];
	dest[1] = a[1] + b[1];
	dest[2] = a[2] + b[2];
}


inline void impSurface::subvec(float* dest, float* a, float* b){
	dest[0] = a[0] - b[0];
	dest[1] = a[1] - b[1];
	dest[2] = a[2] - b[2];
}


inline void impSurface::cross(float* dest, float* a, float* b){
	dest[0] = a[1] * b[2] - b[1] * a[2];
	dest[1] = a[2] * b[0] - b[2] * a[0];
	dest[2] = a[0] * b[1] - b[0] * a[1];
}
