/*****************************************************************************
 *   Copyright (C) 2004-2013 The PaGMO development team,                     *
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

#ifndef PAGMO_PROBLEM_FEASIBILITY_ONLY_H
#define PAGMO_PROBLEM_FEASIBILITY_ONLY_H

#include <string>

#include "../serialization.h"
#include "../types.h"
#include "cec2006.h"
#include "base_meta.h"

namespace pagmo{ namespace problem {

/// Constrained to unconstrained meta-problem
/**
 * Implements a meta-problem class that wraps some other constrained problems,
 * resulting in the same constrained problem with a dummy objective function
 *
 * @author Annalisa Riccardi (nina1983@gmail.com)
 */

class __PAGMO_VISIBLE feasibility_only : public base_meta
{
public:

public:
	//constructors
	feasibility_only(const base & = cec2006(4));

	base_ptr clone() const;
	std::string get_name() const;

protected:
	std::string human_readable_extra() const;
	void objfun_impl(fitness_vector &, const decision_vector &) const;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int)
	{
		ar & boost::serialization::base_object<base_meta>(*this);
	}

};

}} //namespaces

BOOST_CLASS_EXPORT_KEY(pagmo::problem::feasibility_only)

#endif // PAGMO_PROBLEM_FEASIBILITY_ONLY_H
