#pragma once
#ifndef JM_INTERNAL_UNITS_BY_HPP
#define JM_INTERNAL_UNITS_BY_HPP


#include "core_types.hpp"
#include "traits_utils.hpp"


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
        
    protected:
        value_type _value;
        
    public:
        constexpr by() {}
        constexpr by( const T& v ) : _value{ v } {}
        template<
            template< typename > class Other_First_Unit,
            template< typename > class Other_Second_Unit,
            typename O
        >
        constexpr by( const by<
            Other_First_Unit,
            Other_Second_Unit,
            O
        >& o ) : _value{
            static_cast< O >( first_unit< O >{
                Other_First_Unit< O >{ static_cast< O >( o ) }
            } ) * static_cast< O >( second_unit< O >{
                Other_Second_Unit< O >{ static_cast< O >( 1 ) }
            } )
        } {}
        
        // `static_cast<>()` to by types will use the conversion constructor
        template< typename O > explicit constexpr operator O () const
        {
            return static_cast< O >( _value );
        }
    };
    
    // TODO: power<Unit,Exponent,T?>, squared, cubed
} }


#endif
