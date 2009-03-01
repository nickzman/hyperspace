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


//#include <Implicit/impTorus.h>
#include "impTorus.h"


float impTorus::value(float* position){
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

	const float temp(sqrtf(tx*tx + ty*ty) - radius);
	return(thicknessSquared / (temp*temp + tz*tz));
}


// Finding a point inside a torus is trickier than
// finding a point inside a sphere or ellipsoid.
void impTorus::center(float* position){
    position[0] = mat[0] * radius + mat[12];
    position[1] = mat[1] * radius + mat[13];
    position[2] = mat[2] * radius + mat[14];
}


void impTorus::addCrawlPoint(impCrawlPointVector &cpv){
	cpv.push_back(impCrawlPoint(mat[0] * radius + mat[12],
		mat[1] * radius + mat[13],
		mat[2] * radius + mat[14]));
}
