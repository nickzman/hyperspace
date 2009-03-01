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


//#include <rsMath/rsMath.h>
#include "rsMath.h"



rsMatrix::rsMatrix(){
}


rsMatrix::~rsMatrix(){
}


void rsMatrix::identity(){
	m[0] = 1.0f;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = 1.0f;
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = 1.0f;
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}


void rsMatrix::set(float* mat){
	int i;
	for(i=0; i<16; i++)
		m[i] = mat[i];
}


void rsMatrix::get(float* mat){
	int i;
	for(i=0; i<16; i++)
		mat[i] = m[i];
}


void rsMatrix::copy(const rsMatrix &mat){
	int i;
	for(i=0; i<16; i++)
		m[i] = mat[i];
}


void rsMatrix::preMult(const rsMatrix &postMat){
	float preMat[16];

	preMat[0] = m[0];
	preMat[1] = m[1];
	preMat[2] = m[2];
	preMat[3] = m[3];
	preMat[4] = m[4];
	preMat[5] = m[5];
	preMat[6] = m[6];
	preMat[7] = m[7];
	preMat[8] = m[8];
	preMat[9] = m[9];
	preMat[10] = m[10];
	preMat[11] = m[11];
	preMat[12] = m[12];
	preMat[13] = m[13];
	preMat[14] = m[14];
	preMat[15] = m[15];

	m[0] = preMat[0]*postMat[0] + preMat[4]*postMat[1] + preMat[8]*postMat[2] + preMat[12]*postMat[3];
	m[1] = preMat[1]*postMat[0] + preMat[5]*postMat[1] + preMat[9]*postMat[2] + preMat[13]*postMat[3];
	m[2] = preMat[2]*postMat[0] + preMat[6]*postMat[1] + preMat[10]*postMat[2] + preMat[14]*postMat[3];
	m[3] = preMat[3]*postMat[0] + preMat[7]*postMat[1] + preMat[11]*postMat[2] + preMat[15]*postMat[3];
	m[4] = preMat[0]*postMat[4] + preMat[4]*postMat[5] + preMat[8]*postMat[6] + preMat[12]*postMat[7];
	m[5] = preMat[1]*postMat[4] + preMat[5]*postMat[5] + preMat[9]*postMat[6] + preMat[13]*postMat[7];
	m[6] = preMat[2]*postMat[4] + preMat[6]*postMat[5] + preMat[10]*postMat[6] + preMat[14]*postMat[7];
	m[7] = preMat[3]*postMat[4] + preMat[7]*postMat[5] + preMat[11]*postMat[6] + preMat[15]*postMat[7];
	m[8] = preMat[0]*postMat[8] + preMat[4]*postMat[9] + preMat[8]*postMat[10] + preMat[12]*postMat[11];
	m[9] = preMat[1]*postMat[8] + preMat[5]*postMat[9] + preMat[9]*postMat[10] + preMat[13]*postMat[11];
	m[10] = preMat[2]*postMat[8] + preMat[6]*postMat[9] + preMat[10]*postMat[10] + preMat[14]*postMat[11];
	m[11] = preMat[3]*postMat[8] + preMat[7]*postMat[9] + preMat[11]*postMat[10] + preMat[15]*postMat[11];
	m[12] = preMat[0]*postMat[12] + preMat[4]*postMat[13] + preMat[8]*postMat[14] + preMat[12]*postMat[15];
	m[13] = preMat[1]*postMat[12] + preMat[5]*postMat[13] + preMat[9]*postMat[14] + preMat[13]*postMat[15];
	m[14] = preMat[2]*postMat[12] + preMat[6]*postMat[13] + preMat[10]*postMat[14] + preMat[14]*postMat[15];
	m[15] = preMat[3]*postMat[12] + preMat[7]*postMat[13] + preMat[11]*postMat[14] + preMat[15]*postMat[15];
}


void rsMatrix::postMult(const rsMatrix &preMat){
	float postMat[16];

	postMat[0] = m[0];
	postMat[1] = m[1];
	postMat[2] = m[2];
	postMat[3] = m[3];
	postMat[4] = m[4];
	postMat[5] = m[5];
	postMat[6] = m[6];
	postMat[7] = m[7];
	postMat[8] = m[8];
	postMat[9] = m[9];
	postMat[10] = m[10];
	postMat[11] = m[11];
	postMat[12] = m[12];
	postMat[13] = m[13];
	postMat[14] = m[14];
	postMat[15] = m[15];

	m[0] = preMat[0]*postMat[0] + preMat[4]*postMat[1] + preMat[8]*postMat[2] + preMat[12]*postMat[3];
	m[1] = preMat[1]*postMat[0] + preMat[5]*postMat[1] + preMat[9]*postMat[2] + preMat[13]*postMat[3];
	m[2] = preMat[2]*postMat[0] + preMat[6]*postMat[1] + preMat[10]*postMat[2] + preMat[14]*postMat[3];
	m[3] = preMat[3]*postMat[0] + preMat[7]*postMat[1] + preMat[11]*postMat[2] + preMat[15]*postMat[3];
	m[4] = preMat[0]*postMat[4] + preMat[4]*postMat[5] + preMat[8]*postMat[6] + preMat[12]*postMat[7];
	m[5] = preMat[1]*postMat[4] + preMat[5]*postMat[5] + preMat[9]*postMat[6] + preMat[13]*postMat[7];
	m[6] = preMat[2]*postMat[4] + preMat[6]*postMat[5] + preMat[10]*postMat[6] + preMat[14]*postMat[7];
	m[7] = preMat[3]*postMat[4] + preMat[7]*postMat[5] + preMat[11]*postMat[6] + preMat[15]*postMat[7];
	m[8] = preMat[0]*postMat[8] + preMat[4]*postMat[9] + preMat[8]*postMat[10] + preMat[12]*postMat[11];
	m[9] = preMat[1]*postMat[8] + preMat[5]*postMat[9] + preMat[9]*postMat[10] + preMat[13]*postMat[11];
	m[10] = preMat[2]*postMat[8] + preMat[6]*postMat[9] + preMat[10]*postMat[10] + preMat[14]*postMat[11];
	m[11] = preMat[3]*postMat[8] + preMat[7]*postMat[9] + preMat[11]*postMat[10] + preMat[15]*postMat[11];
	m[12] = preMat[0]*postMat[12] + preMat[4]*postMat[13] + preMat[8]*postMat[14] + preMat[12]*postMat[15];
	m[13] = preMat[1]*postMat[12] + preMat[5]*postMat[13] + preMat[9]*postMat[14] + preMat[13]*postMat[15];
	m[14] = preMat[2]*postMat[12] + preMat[6]*postMat[13] + preMat[10]*postMat[14] + preMat[14]*postMat[15];
	m[15] = preMat[3]*postMat[12] + preMat[7]*postMat[13] + preMat[11]*postMat[14] + preMat[15]*postMat[15];
}


float rsMatrix::determinant(){
	return(m[0] * m[5] * m[10] * m[15]
		+ m[4] * m[9] * m[14] * m[3]
		+ m[8] * m[13] * m[2] * m[7]
		+ m[12] * m[1] * m[6] * m[11]
		- m[3] * m[6] * m[9] * m[12]
		- m[7] * m[10] * m[13] * m[0]
		- m[11] * m[14] * m[1] * m[4]
		- m[15] * m[2] * m[5] * m[8]);
}


void rsMatrix::makeTrans(float x, float y, float z){
	m[0] = 1.0f;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = 1.0f;
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = 1.0f;
	m[11] = 0.0f;
	m[12] = x;
	m[13] = y;
	m[14] = z;
	m[15] = 1.0f;
}

void rsMatrix::makeTrans(float *p){
	m[0] = 1.0f;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = 1.0f;
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = 1.0f;
	m[11] = 0.0f;
	m[12] = p[0];
	m[13] = p[1];
	m[14] = p[2];
	m[15] = 1.0f;
}

void rsMatrix::makeTrans(const rsVec &vec){
	m[0] = 1.0f;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = 1.0f;
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = 1.0f;
	m[11] = 0.0f;
	m[12] = vec[0];
	m[13] = vec[1];
	m[14] = vec[2];
	m[15] = 1.0f;
}


void rsMatrix::makeScale(float s){
	m[0] = s;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = s;
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = s;
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}

void rsMatrix::makeScale(float x, float y, float z){
	m[0] = x;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = y;
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = z;
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}

void rsMatrix::makeScale(float* s){
	m[0] = s[0];
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = s[1];
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = s[2];
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}

void rsMatrix::makeScale(const rsVec &vec){
	m[0] = vec[0];
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = vec[1];
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = vec[2];
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}


void rsMatrix::makeRot(float a, float x, float y, float z){
	rsQuat q;
	q.make(a, x, y, z);
	q.toMat(m);
}

void rsMatrix::makeRot(float a, const rsVec &v){
	rsQuat q;
	q.make(a, v);
	q.toMat(m);
}

void rsMatrix::makeRot(rsQuat &q){
	q.toMat(m);
}


int rsMatrix::invert(const rsMatrix &mat){
	float rmat[4][8];
	float a, b;
	int i, j, k;

	// initialize reduction matrix
	rmat[0][0] = mat[0];
	rmat[1][0] = mat[1];
	rmat[2][0] = mat[2];
	rmat[3][0] = mat[3];
	rmat[0][1] = mat[4];
	rmat[1][1] = mat[5];
	rmat[2][1] = mat[6];
	rmat[3][1] = mat[7];
	rmat[0][2] = mat[8];
	rmat[1][2] = mat[9];
	rmat[2][2] = mat[10];
	rmat[3][2] = mat[11];
	rmat[0][3] = mat[12];
	rmat[1][3] = mat[13];
	rmat[2][3] = mat[14];
	rmat[3][3] = mat[15];
	rmat[0][4] = 1.0f;
	rmat[1][4] = 0.0f;
	rmat[2][4] = 0.0f;
	rmat[3][4] = 0.0f;
	rmat[0][5] = 0.0f;
	rmat[1][5] = 1.0f;
	rmat[2][5] = 0.0f;
	rmat[3][5] = 0.0f;
	rmat[0][6] = 0.0f;
	rmat[1][6] = 0.0f;
	rmat[2][6] = 1.0f;
	rmat[3][6] = 0.0f;
	rmat[0][7] = 0.0f;
	rmat[1][7] = 0.0f;
	rmat[2][7] = 0.0f;
	rmat[3][7] = 1.0f;

	// perform reductions
	for(i=0; i<4; i++){
		a = rmat[i][i];
		if(a == 0.0f)  // matrix is singular, can't be reduced
			return 0;
		else{
			a = 1.0f / a;
			for(j=0; j<8; j++)
				rmat[i][j] = rmat[i][j] * a;
			for(k=0; k<4; k++){
				if((k-i) != 0){
					b = rmat[k][i];
					for(j=0; j<8; j++)
						rmat[k][j] = rmat[k][j] - b * rmat[i][j];
				}
			}
		}
	}

	// extract the inverted matrix
	m[0] = rmat[0][4];
	m[1] = rmat[1][4];
	m[2] = rmat[2][4];
	m[3] = rmat[3][4];
	m[4] = rmat[0][5];
	m[5] = rmat[1][5];
	m[6] = rmat[2][5];
	m[7] = rmat[3][5];
	m[8] = rmat[0][6];
	m[9] = rmat[1][6];
	m[10] = rmat[2][6];
	m[11] = rmat[3][6];
	m[12] = rmat[0][7];
	m[13] = rmat[1][7];
	m[14] = rmat[2][7];
	m[15] = rmat[3][7];

	return 1;
}


void rsMatrix::rotationInvert(const rsMatrix &mat){
	float det = mat[0] * mat[5] * mat[10]
		+ mat[4] * mat[9] * mat[2]
		+ mat[8] * mat[1] * mat[6]
		- mat[2] * mat[5] * mat[8]
		- mat[6] * mat[9] * mat[0]
		- mat[10] * mat[1] * mat[4];

	m[0] = (mat[5] * mat[10] - mat[6] * mat[9]) / det;
	m[1] = (mat[6] * mat[8] - mat[4] * mat[10]) / det;
	m[2] = (mat[4] * mat[9] - mat[5] * mat[8]) / det;
	m[4] = (mat[9] * mat[2] - mat[10] * mat[1]) / det;
	m[5] = (mat[10] * mat[0] - mat[8] * mat[2]) / det;
	m[6] = (mat[8] * mat[1] - mat[9] * mat[0]) / det;
	m[8] = (mat[1] * mat[6] - mat[2] * mat[5]) / det;
	m[9] = (mat[2] * mat[4] - mat[0] * mat[6]) / det;
	m[10] = (mat[0] * mat[5] - mat[1] * mat[4]) / det;
	m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0f;
	m[15] = 1.0f;
}


void rsMatrix::fromQuat(const rsQuat &q){
	float s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

	// must have an axis
	if(q[0] == 0.0f && q[1] == 0.0f && q[2] == 0.0f){
		identity();
		return;
	}

	s = 2.0f / (q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	xs = q[0] * s;
	ys = q[1] * s;
	zs = q[2] * s;
	wx = q[3] * xs;
	wy = q[3] * ys;
	wz = q[3] * zs;
	xx = q[0] * xs;
	xy = q[0] * ys;
	xz = q[0] * zs;
	yy = q[1] * ys;
	yz = q[1] * zs;
	zz = q[2] * zs;

	m[0] = 1.0f - yy - zz;
	m[1] = xy + wz;
	m[2] = xz - wy;
	m[3] = 0.0f;
	m[4] = xy - wz;
	m[5] = 1.0f - xx - zz;
	m[6] = yz + wx;
	m[7] = 0.0f;
	m[8] = xz + wy;
	m[9] = yz - wx;
	m[10] = 1.0f - xx - yy;
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}



rsMatrix & rsMatrix::operator = (const rsMatrix &mat){
	m[0]=mat[0]; m[1]=mat[1]; m[2]=mat[2]; m[3]=mat[3];
	m[4]=mat[4]; m[5]=mat[5]; m[6]=mat[6]; m[7]=mat[7];
	m[8]=mat[8]; m[9]=mat[9]; m[10]=mat[10]; m[11]=mat[11];
	m[12]=mat[12]; m[13]=mat[13]; m[14]=mat[14]; m[15]=mat[15];
	return *this;
}
