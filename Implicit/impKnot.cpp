/*
 * Copyright (C) 2005  Terence M. Welsh
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


//#include <Implicit/impKnot.h>
#include "impKnot.h"


float impKnot::value(float* position){
	const float tx(position[0] * invmat[0]
		+ position[1] * invmat[4]
		+ position[2] * invmat[8]
		+ invmat[12]);
	const float ty(position[0] * invmat[1]
		+ position[1] * invmat[5]
		+ position[2] * invmat[9]
		+ invmat[13]);
	const float tz(position[0] * invmat[2]
		+ position[1] * invmat[6]
		+ position[2] * invmat[10]
		+ invmat[14]);

	const float temp(sqrtf(tx*tx + ty*ty) - radius1);
	const float lat(atan2f(ty, tx) * twistsOverCoils);
	float retval(0.0f);
	for(int i=0; i<coils; i++){
		const float lon(lat + lat_offset * float(i));
		const float hor(temp - cosf(lon) * radius2);
		const float ver(tz - sinf(lon) * radius2);
		retval += thicknessSquared / (hor * hor + ver * ver);
	}
	return retval;
}


// Finding a point inside a knot is trickier than
// finding a point inside a sphere or ellipsoid.
void impKnot::center(float* position){
    position[0] = mat[0] * (radius1 + radius2) + mat[12];
    position[1] = mat[1] * (radius1 + radius2) + mat[13];
    position[2] = mat[2] * (radius1 + radius2) + mat[14];
}


void impKnot::addCrawlPoint(impCrawlPointVector &cpv){
	cpv.push_back(impCrawlPoint(mat[0] * (radius1 + radius2) + mat[12],
		mat[1] * (radius1 + radius2) + mat[13],
		mat[2] * (radius1 + radius2) + mat[14]));
}
