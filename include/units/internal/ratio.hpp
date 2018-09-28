#pragma once
#ifndef JM_INTERNAL_UNITS_RATIO_HPP
#define JM_INTERNAL_UNITS_RATIO_HPP


#include "core_types.hpp"
#include "traits_utils.hpp"


namespace JadeMatrix { namespace units // Dimensionless values /////////////////
{
    template< typename T > class ratio
    {
    public:
        using value_type = T;
        
    protected:
        value_type _value;
        
    public:
        constexpr ratio() {}
        constexpr ratio( const value_type& v ) : _value{ v } {}
        // Implicit conversion from unit/unit of same unit
        template<
            template< typename > class Unit,
            typename O
        > constexpr ratio( const per< Unit, Unit, O >& o ) :
            _value{ static_cast< O >( o ) }
        {}
        
        // Ratio can be implicitly converted to its base type
        constexpr operator value_type () const
        {
            return _value;
        }
        template<
            typename O = value_type,
            typename = typename std::enable_if< !is_unit< O >::value >::type
        > explicit constexpr operator O () const
        {
            return static_cast< O >( _value );
        }
    };
} }


#endif
