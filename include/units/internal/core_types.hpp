#pragma once
#ifndef JM_UNITS_CORE_TYPES_HPP
#define JM_UNITS_CORE_TYPES_HPP


#include "../scales.hpp"


namespace JadeMatrix { namespace units
{
    template<
        typename Traits,
        typename Scale,
        typename T = void
    > class unit;
    
    template<
        template< typename > typename Numer_Unit,
        template< typename > typename Denom_Unit,
        typename T = void
    > class per;
    
    template<
        template< typename > typename First_Unit,
        template< typename > typename Second_Unit,
        typename T = void
    > class by;
    
    template< typename T = void > class ratio;
} }


#endif
