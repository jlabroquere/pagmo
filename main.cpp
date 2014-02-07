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

#include <iostream>
#include <iomanip>
#include "src/pagmo.h"

using namespace pagmo;

// Example in C++ of the use of PaGMO 1.1.5

int main()
{
	/*
	//We instantiate the problem Schwefel with diemnsion 50
	pagmo::problem::schwefel prob(50);
	//We instantiate the algorithm differential evolution with 500 generations
	pagmo::algorithm::de algo(3000);

	//1 - Evolution takes place on the same thread as main
	//We instantiate a population containing 20 candidate solutions to the Schwefel problem
	pagmo::population pop(prob,20);
	algo.evolve(pop);
	
	std::cout << "Evolve method of the algorithm: " << pop.champion().f << std::endl; 
	
	//2 - Evolution takes place on a separate thread
	//We instantiate an island containing 20 candidate solutions to the Schwefel problem
	pagmo::island isl(algo,prob,20);
	isl.evolve();
	
	std::cout << "Evolve method of the island: " << isl.get_population().champion().f << std::endl; 

	//3 - 8 Evolutions take place in parallel on 8 separte islands containing, each, 20
	// candidate solutions to the Schwefel problem
	pagmo::archipelago archi(algo,prob,8,20);
	archi.evolve();

	std::vector<double> temp;
	for (archipelago::size_type i = 0; i < archi.get_size(); ++i) {
		temp.push_back(archi.get_island(i)->get_population().champion().f[0]);
	}
	std::cout << "Evolve method of the archipelago: " << *std::min_element(temp.begin(),temp.end()) << std::endl; 
	
	//4 - 8 Evolutions take place in parallel on 8 separte islands with migration
	pagmo::algorithm::de algo2(300);
	pagmo::topology::one_way_ring topo;
	pagmo::archipelago archi2(algo2,prob,8,20,topo);
	archi2.evolve(10);
	
	temp.clear();
	for (archipelago::size_type i = 0; i < archi.get_size(); ++i) {
		temp.push_back(archi2.get_island(i)->get_population().champion().f[0]);
	}
	std::cout << "Evolve method of the archipelago (with migration): " << *std::min_element(temp.begin(),temp.end()) << std::endl; 
	return 0;
	*/

	std::vector<std::vector<double> > retval;
	std::vector<double> dumb(2);
	dumb[0] = 180;dumb[1] = 200;
	retval.push_back(dumb);
	dumb[0] = 0.1;dumb[1] = 5;
	retval.push_back(dumb);
	dumb[0] = 10;dumb[1] = 150;
	retval.push_back(dumb);
	dumb[0] = 10;dumb[1] = 40;
	retval.push_back(dumb);

	std::vector<kep_toolbox::planet_ptr> retval_planet;
	retval_planet.push_back(kep_toolbox::planet_js("callisto").clone());
	retval_planet.push_back(kep_toolbox::planet_js("ganymede").clone());
	retval_planet.push_back(kep_toolbox::planet_js("ganymede").clone());
	retval_planet.push_back(kep_toolbox::planet_js("ganymede").clone());

	decision_vector x(18);
	for(int i=0; i<18; i++) 
		x[i] = 0.5;

	pagmo::problem::mga_incipit_cstrs prob(retval_planet, kep_toolbox::epoch(10460.0), kep_toolbox::epoch(10480.0),retval,365,std::vector<double>(3,2.0),0.3,10000000,0.1,0.0);

	prob.compute_constraints(x);

	int uscita;
	std::cin>>uscita;
}
