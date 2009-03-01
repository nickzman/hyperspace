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


#ifndef RSMATH_H
#define RSMATH_H



#include <stdlib.h>
/*#include <rsMath/rsVec.h>
#include <rsMath/rsMatrix.h>
#include <rsMath/rsQuat.h>
#include <rsMath/rsTrigonometry.h>*/
#include "rsVec.h"
#include "rsMatrix.h"
#include "rsQuat.h"
#include "rsTrigonometry.h"



#define RSEPSILON 0.000001f
#define RS_PIo2 1.5707963268f
#define RS_PI 3.14159265359f
#define RS_PIx2 6.28318530718f
#define RS_RAD2DEG 57.2957795131f



// Useful random number functions
// Don't forget to initialize with srand()
inline int rsRandi(int x){
	return rand() % x;
}


inline float rsRandf(float x){
	return x * (float(rand()) / float(RAND_MAX));
}



#endif
