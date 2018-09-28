#pragma once
#ifndef JM_UNITS_CORE_TYPES_HPP
#define JM_UNITS_CORE_TYPES_HPP


#include "scales.hpp"


namespace JadeMatrix { namespace units
{
    template<
        typename Traits,
        typename T,
        typename Scale = unit_scale
    > class unit;
    
    template<
        template< typename > typename Numer_Unit,
        template< typename > typename Denom_Unit,
        typename T
    > class per;
    
    template<
        template< typename > typename First_Unit,
        template< typename > typename Second_Unit,
        typename T
    > class by;
    
    template< typename T > class ratio;
} }


#endif
