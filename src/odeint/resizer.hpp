/* Boost odeint/resizer.hpp header file
 
 Copyright 2009 Karsten Ahnert
 Copyright 2009 Mario Mulansky
 
 This file includes resizer functionality for containers
*/

#ifndef BOOST_NUMERIC_ODEINT_RESIZER_HPP
#define BOOST_NUMERIC_ODEINT_RESIZER_HPP

#include <tr1/array>

namespace odeint {

    template< class Container > 
    class resizer
    {
    public:

	typedef Container container_type;



	
    public:
        void resize( const container_type &x , container_type &dxdt ) const
        {
            dxdt.resize( x.size() );
        }
        
        bool same_size( const container_type &x1 , const container_type &x2 ) const
        {
            return (x1.size() == x2.size());
        }

	void adjust_size( const container_type &x1 , container_type &x2 ) const
        {
	    if( !same_size( x1 , x2 ) ) resize( x1 , x2 );
	}
    };



    /* Template Specialization for fixed size array - no resizing can happen */
    template< class T , size_t N >
    class resizer< std::tr1::array< T , N > >
    {
    public:

	typedef std::tr1::array< T , N > container_type;


    public:

        void resize( const container_type &x , container_type &dxdt ) const
        {
            throw; // should never be called
        }

        const bool same_size( const container_type &x1 , const container_type &x2 ) const
        {
            return true; // if this was false, the code wouldn't compile
        }

	void adjust_size( const container_type &x1 , container_type &x2 ) const
        {
	    if( !same_size( x1 , x2 ) ) throw;
	}
    };


} // namespace odeint


#endif // BOOST_NUMERIC_ODEINT_RESIZER_HPP
