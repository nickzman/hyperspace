/*
 * Copyright (C) 1999-2005  Terence M. Welsh
 *
 * This file is part of rsMath.
 *
 * rsMath is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * rsMath is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef RSMATRIX_H
#define RSMATRIX_H



class rsVec;
class rsQuat;



class rsMatrix{
public:
	float m[16];
	// 1 0 0 x   0 4 8  12  <-- Column order matrix just like OpenGL
	// 0 1 0 y   1 5 9  13
	// 0 0 1 z   2 6 10 14
	// 0 0 0 1   3 7 11 15

	rsMatrix();
	~rsMatrix();
	void identity();
	void set(float* mat);
	void get(float* mat);
	void copy(const rsMatrix &mat);
	void preMult(const rsMatrix &postMat);
	void postMult(const rsMatrix &preMat);
	void makeTrans(float x, float y, float z);
	void makeTrans(float* p);
	void makeTrans(const rsVec &vec);
	void makeScale(float s);
	void makeScale(float x, float y, float z);
	void makeScale(float* s);
	void makeScale(const rsVec &vec);
	void makeRot(float a, float x, float y, float z);	// angle, axis
	void makeRot(float a, const rsVec &v);	// angle, axis
	void makeRot(rsQuat &q);
	float determinant();
	int invert(const rsMatrix &mat);  // general matrix inversion
	void rotationInvert(const rsMatrix &mat);  // rotation matrix inversion
	void fromQuat(const rsQuat &q);

	const float & operator [] (int i) const {return m[i];}
	rsMatrix & operator = (const rsMatrix &mat);
};



#endif
