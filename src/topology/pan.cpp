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

#include <string>

#include "base.h"
#include "pan.h"

namespace pagmo { namespace topology {

/// Constructor.
pan::pan():base() {}

base_ptr pan::clone() const
{
	return base_ptr(new pan(*this));
}

void pan::connect(const vertices_size_type &n)
{
	switch(n)
	{
		case 0:
			break;
		case 1:
			// Add connection from the newly added node to the zeroth.
			add_edge(1,0);
			break;
		case 2:
			add_edge(1,2);
			add_edge(2,1);
			break;
		case 3:
			add_edge(2,3);
			add_edge(3,2);
			add_edge(1,3);
			add_edge(3,1);
			break;
		default:
			remove_edge(n - 1,1);
			remove_edge(1,n - 1);
			add_edge(n - 1,n);
			add_edge(n,n - 1);
			add_edge(1,n);
			add_edge(n,1);
	}
}

std::string pan::get_name() const
{
	return "Pan graph";
}

}} //namespaces