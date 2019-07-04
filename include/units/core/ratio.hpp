#pragma once
#ifndef JM_UNITS_CORE_RATIO_HPP
#define JM_UNITS_CORE_RATIO_HPP


#include "internal/core_type_detection.hpp"
#include "internal/core_types.hpp"

#include <type_traits>  // enable_if


namespace JadeMatrix { namespace units // Dimensionless values /////////////////
{
    template< typename T > class ratio
    {
    public:
        using value_type = T;
        template< typename O > using unit_type = ratio< O >;
        
    protected:
        value_type _value;
        
    public:
        constexpr ratio() {}
        constexpr ratio( const value_type& v ) : _value{ v } {}
        
        // TODO: copy vs. move versions (needs template friend ratio)
        template<
            typename O,
            typename = typename std::enable_if<
                internal::is_ratio< internal::reduced< O > >::value
            >::type
        > constexpr ratio( const O& o ) :
            _value{ static_cast< typename O::value_type >( o ) }
        {}
        
        // Ratio can be implicitly converted to its base type
        constexpr operator value_type () const
        {
            return _value;
        }
        template<
            typename O = value_type,
            typename = typename std::enable_if<
                !internal::is_unit< O >::value
            >::type
        > explicit constexpr operator O () const
        {
            return static_cast< O >( _value );
        }
    };
    
    template<> class ratio< void >
    {
    public:
        using value_type = void;
        template< typename O > using unit_type = ratio< O >;
        
        ratio() = delete;
        ~ratio() = delete;
    };
} }


#endif
