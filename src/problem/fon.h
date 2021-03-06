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

#ifndef PAGMO_PROBLEM_FON_H
#define PAGMO_PROBLEM_FON_H

#include <string>

#include "../serialization.h"
#include "../types.h"
#include "base.h"

namespace pagmo{ namespace problem {

/// Fonseca and Fleming's study
/**
 *
 * This is a box-constrained continuous three-dimension multi-objecive problem.
 * \f[
 * 	F_1 \left(x\right) = 1 - \exp(-\sum_{i=1}^3 (x_i - 1 / \sqrt(3))^2
 * \f]
 * \f[
 * 	F_2 \left(x\right) = 1 - \exp(-\sum_{i=1}^3 (x_i + 1 / \sqrt(3))^2  x \in \left[ -4,4 \right].
 * \f]
 * The optimal solutions (minimization problem) are \f$ x_1 = x_2 = x_3 \in \left[-1/ \sqrt(3) , +1/ \sqrt(3)  \right] \f$
 *
 * @see http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.55.5395&rep=rep1&type=ps
 * @author Andrea Mambrini (andrea.mambrini@gmail.com)
 */

class __PAGMO_VISIBLE fon : public base
{
	public:
		fon();
		base_ptr clone() const;
		std::string get_name() const;
	protected:
		void objfun_impl(fitness_vector &, const decision_vector &) const;
	private:
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive &ar, const unsigned int)
		{
			ar & boost::serialization::base_object<base>(*this);
		}
};

}} //namespaces

BOOST_CLASS_EXPORT_KEY(pagmo::problem::fon);

#endif // PAGMO_PROBLEM_FON_H
