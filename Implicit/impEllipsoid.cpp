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


//#include <Implicit/impEllipsoid.h>
#include "impEllipsoid.h"


float impEllipsoid::value(float* position){
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
	return(1.0f / (tx*tx + ty*ty + tz*tz));
}
