/*****************************************************************************
 *   Copyright (C) 2004-2009 The PaGMO development team,                     *
 *   Advanced Concepts Team (ACT), European Space Agency (ESA)               *
 *   http://apps.sourceforge.net/mediawiki/pagmo                             *
 *   http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Developers  *
 *   http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Credits     *
 *   act@esa.int                                                             *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.               *
 *****************************************************************************/

#ifndef ARRAY3D_OPERATIONS_H
#define ARRAY3D_OPERATIONS_H

#include"../astro_constants.h"

namespace kep_toolbox {
	inline void sum(array3D& out, const array3D& v1,const array3D& v2){
		out[0] = v1[0]+v2[0];
		out[1] = v1[1]+v2[1];
		out[2] = v1[2]+v2[2];
	}
	inline void diff(array3D& out, const array3D& v1,const array3D& v2){
		out[0] = v1[0]-v2[0];
		out[1] = v1[1]-v2[1];
		out[2] = v1[2]-v2[2];
	}
	inline double dot(const array3D& v1,const array3D& v2){
		return (v1[0]*v2[0] +  v1[1]*v2[1]+ v1[2]*v2[2]);
	}
	inline double norm(const array3D& v1){
		return sqrt(v1[0]*v1[0] +  v1[1]*v1[1]+ v1[2]*v1[2]);
	}
	inline void unit_vector(array3D& out, const array3D& in){
		double dummy = norm(in);
		out[0] = in[0] / dummy;
		out[1] = in[1] / dummy;
		out[2] = in[2] / dummy;
	}
	inline void cross(array3D& out, const array3D& v1,const array3D& v2){
		out[0] = v1[1]*v2[2] - v1[2]*v2[1];
		out[1] = v1[2]*v2[0] - v1[0]*v2[2];
		out[2] = v1[0]*v2[1] - v1[1]*v2[0];
	}
}

#endif // ARRAY3D_OPERATIONS_H