#pragma once
#ifndef JM_UNITS_CORE_BY_HPP
#define JM_UNITS_CORE_BY_HPP


#include "internal/core_type_detection.hpp"
#include "internal/core_types.hpp"

#include <type_traits>  // enable_if


namespace JadeMatrix { namespace units // Unit multiplied by another ///////////
{
    template<
        template< typename > class First_Unit,
        template< typename > class Second_Unit,
        typename T
    > class by
    {
    public:
        template< typename O > using  first_unit =  First_Unit< O >;
        template< typename O > using second_unit = Second_Unit< O >;
        using value_type = T;
        template< typename O > using unit_type = by<
            First_Unit,
            Second_Unit,
            O
        >;
        
    protected:
        value_type _value;
        
        template<
            template< typename > class OtherUnit
        > using convert_from = internal::conversion< unit_type, OtherUnit >;
        
    public:
        constexpr by() {}
        constexpr by( const T& v ) : _value{ v } {}
        
        template<
            typename O,
            typename = typename std::enable_if<
                convert_from< O::template unit_type >::exists
            >::type
        > constexpr by( const O& o ) :
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
        template< typename > class First_Unit,
        template< typename > class Second_Unit
    > class by< First_Unit, Second_Unit, void >
    {
    public:
        template< typename O > using  first_unit =  First_Unit< O >;
        template< typename O > using second_unit = Second_Unit< O >;
        using value_type = void;
        template< typename O > using unit_type = by<
            first_unit,
            second_unit,
            O
        >;
        
        by() = delete;
        ~by() = delete;
    };
    
    // TODO: power<Unit,Exponent,T?>, squared, cubed
} }


#endif
