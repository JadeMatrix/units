#pragma once
#ifndef UNITS_STRING_CONVERSIONS_HPP
#define UNITS_STRING_CONVERSIONS_HPP


#include "unit.hpp"

#include <string>


namespace units
{
    template< typename T >
    std::string to_string( const T& v )
    {
        using std::to_string;
        return std::to_string( v );
    }
    
    template< typename T, class Traits >
    std::string to_string( const unit< T, Traits >& v )
    {
        return to_string( ( T )v ) + Traits::unit_string();
    }
}


#endif
