/*
 * Copyright (C) 1999-2010  Terence M. Welsh
 *
 * This file is part of rsMath.
 *
 * rsMath is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
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


#ifndef RSMATH_H
#define RSMATH_H


#include <stdlib.h>
#include <math.h>
#include <rsMath/rsVec.h>
#include <rsMath/rsVec4.h>
#include <rsMath/rsMatrix.h>
#include <rsMath/rsQuat.h>
#include <rsMath/rsTrigonometry.h>
#ifdef __SSE__
#include <xmmintrin.h>
#endif


#define RS_EPSILON 0.000001f
#define RS_PIo2 1.57079632679f
#define RS_PI 3.14159265359f
#define RS_PIx2 6.28318530718f
#define RS_DEG2RAD 0.0174532925f
#define RS_RAD2DEG 57.2957795131f



// Useful random number functions
// Don't forget to initialize with srand()
inline int rsRandi(int x){
	return rand() % x;
}


inline float rsRandf(float x){
	return x * (float(rand()) / float(RAND_MAX));
}


inline float rsSqrtf(const float& x){
#ifdef __SSE__
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(x)));
#else
	//return powf(x, 0.5f);
	return sqrtf(x);
#endif
}


inline float rsInvSqrtf(const float& x){
#ifdef __SSE__
	return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x)));
#else
	//return 1.0f / powf(x, 0.5f);
	return 1.0f / sqrtf(x);
#endif
}
/*typedef union {
	float f;
	int i;
} float_or_int;

inline float rsInvSqrtf(const float& x){
	float_or_int tmp;
	tmp.f = x;
	tmp.i = 0x5f3759df - (tmp.i >> 1);
	return tmp.f * (1.5f - 0.5f * x * tmp.f * tmp.f);
}*/


#endif
