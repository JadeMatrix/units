#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_CORE_TYPES_HPP
#define JM_UNITS_CORE_INTERNAL_CORE_TYPES_HPP


namespace JadeMatrix { namespace units
{
    // Forward declarations of core types for reference by internal machinery
    
    template<
        typename Traits,
        typename Scale,
        typename T = void
    > class unit;
    
    template<
        template< typename > class Numer_Unit,
        template< typename > class Denom_Unit,
        typename T = void
    > class per;
    
    template<
        template< typename > class First_Unit,
        template< typename > class Second_Unit,
        typename T = void
    > class by;
    
    template< typename T = void > class ratio;
} }


#endif
