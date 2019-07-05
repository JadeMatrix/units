#pragma once
#ifndef JM_UNITS_CORE_PER_HPP
#define JM_UNITS_CORE_PER_HPP


#include "internal/conversion.hpp"
#include "internal/core_type_detection.hpp"
#include "internal/core_types.hpp"

#include <type_traits>  // enable_if


namespace JadeMatrix { namespace units // Unit divided by another //////////////
{
    template<
        template< typename > class Numer_Unit,
        template< typename > class Denom_Unit,
        typename T
    > class per
    {
    public:
        template< typename O > using numer_unit = Numer_Unit< O >;
        template< typename O > using denom_unit = Denom_Unit< O >;
        using value_type = T;
        template< typename O > using unit_type = per<
            Numer_Unit,
            Denom_Unit,
            O
        >;
        
    protected:
        value_type _value;
        
        template<
            template< typename > class OtherUnit
        > using convert_from = internal::conversion< unit_type, OtherUnit >;
        
    public:
        constexpr per() {}
        constexpr per( const value_type& v ) : _value{ v } {}
        
        template<
            typename O,
            typename = typename std::enable_if<
                convert_from< O::template unit_type >::exists
            >::type
        > constexpr per( const O& o ) :
            _value( convert_from< O::template unit_type >::apply( o ) )
        {}
        
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
    
    template<
        template< typename > class Numer_Unit,
        template< typename > class Denom_Unit
    > class per< Numer_Unit, Denom_Unit, void >
    {
    public:
        template< typename O > using numer_unit = Numer_Unit< O >;
        template< typename O > using denom_unit = Denom_Unit< O >;
        using value_type = void;
        template< typename O > using unit_type = per<
            numer_unit,
            denom_unit,
            O
        >;
        
        per() = delete;
        ~per() = delete;
    };
} }


#endif
