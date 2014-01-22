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

#ifndef PAGMO_PROBLEM_CEC2010_H
#define PAGMO_PROBLEM_CEC2010_H

#include "../config.h"
#include "../serialization.h"
#include "../types.h"
#include "base.h"

namespace pagmo{ namespace problem {

/// The CEC 2010 problems: Constrained Real-Parameter Optimization
/**
 *
 * This class allows to instantiate any of the 18 problems of the competition
 * on constrained real-parameter optimization problems that was organized in the
 * framework of the 2010 IEEE Congress on Evolutionary Computation.
 *
 * NOTE: all problems are constrained, continuous, single objective problems.
 *
 * @see http://www3.ntu.edu.sg/home/epnsugan/index_files/CEC10-Const/CEC10-Const.htm
 *
 * @author Annalisa Riccardi (nina1983@gmail.com)
 * @author Jeremie Labroquere (jeremie.labroquere@gmail.com)
 */

class __PAGMO_VISIBLE cec2010 : public base
{
    public:
		cec2010(int = 1, int = 30);
        base_ptr clone() const;
		void compute_error(decision_vector&, const decision_vector&) const;

		void set_best_o(const decision_vector&);

		std::string get_name() const;
		const decision_vector& get_best_o(void) const;

    protected:
        void objfun_impl(fitness_vector &, const decision_vector &) const;
        void compute_constraints_impl(constraint_vector &, const decision_vector &) const;

    private:
        void g01_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g01_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g02_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g02_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g03_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g03_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g04_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g04_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g05_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g05_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g06_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g06_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g07_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g07_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g08_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g08_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g09_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g09_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g10_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g10_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g11_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g11_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g12_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g12_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g13_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g13_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g14_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g14_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g15_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g15_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g16_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g16_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g17_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g17_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;
        void g18_objfun_impl(fitness_vector &, const decision_vector &) const;
        void g18_compute_constraints_impl(constraint_vector &c, const decision_vector &x) const;

        void initialize_o(void);

        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive &ar, const unsigned int)
        {
            ar & boost::serialization::base_object<base>(*this);
            ar & const_cast<unsigned int&>(m_problem_number);
			ar & m_best_o;
        }

		const unsigned int m_problem_number;
        const unsigned int m_problem_dimension;
		decision_vector m_best_o;

		// no need to serialize these static vectors
        static const constraint_vector::size_type m_problems_c_dimension[];
        static const constraint_vector::size_type m_problems_ic_dimension[];


};

}} //namespaces

BOOST_CLASS_EXPORT_KEY(pagmo::problem::cec2010)

#endif
