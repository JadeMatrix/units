#pragma once
#ifndef JM_UNITS_UNIT_HPP
#define JM_UNITS_UNIT_HPP


#include "core_types.hpp"
#include "scales.hpp"
#include "traits_utils.hpp"

#include <type_traits>


namespace JadeMatrix { namespace units // Base unit classes ////////////////////
{
    template<
        typename Traits,
        typename T,
        typename Scale
    > class unit
    {
    public:
        using traits_type = Traits;
        using  value_type = T;
        using  scale_type = Scale;
        
    protected:
        value_type _value;
        
        using convert = traits_convert< traits_type, value_type >;
        
        template<
            typename Other_Traits,
            typename O,
            typename Other_Scale
        > static constexpr auto from(
            const unit< Other_Traits, O, Other_Scale >& o
        ) -> typename std::enable_if<
            true,
            value_type
        >::type
        {
            return scale_type::scale(
                Other_Scale::unscale(
                    convert::template from< Other_Traits >(
                        static_cast< O >( o )
                    )
                )
            );
        }
        
    public:
        constexpr unit() {}
        constexpr unit( const value_type& v ) : _value{ v } {}
        template<
            typename Other_Traits,
            typename O,
            typename Other_Scale
        > constexpr unit( const unit< Other_Traits, O, Other_Scale >& o ) :
            _value{ from( o ) }
        {}
        // Allow custom conversions if specialization available in unit traits
        template< typename In > constexpr unit(
            const typename std::enable_if< (
                !is_unit< In >::value
                && !std::is_arithmetic< In >::value
            ), In
        >::value& v ) :
            _value{ traits_type::from( v ) }
        {}
        
        // `static_cast<>()` to unit types will use the conversion constructor
        template< typename O > explicit constexpr operator O () const
        {
            return static_cast< O >( _value );
        }
    };
    
    
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
        
    protected:
        value_type _value;
        
    public:
        constexpr per() {}
        constexpr per( const value_type& v ) : _value{ v } {}
        template<
            template< typename > class Other_Numer_Unit,
            template< typename > class Other_Denom_Unit,
            typename O
        >
        constexpr per( const per<
            Other_Numer_Unit,
            Other_Denom_Unit,
            O
        >& o ) : _value{
            static_cast< O >( numer_unit< O >{
                Other_Numer_Unit< O >{ static_cast< O >( o ) }
            } ) / static_cast< O >( denom_unit< O >{
                Other_Denom_Unit< O >{ static_cast< O >( 1 ) }
            } )
        }
        {}
        
        // `static_cast<>()` to per types will use the conversion constructor
        template< typename O > explicit constexpr operator O () const
        {
            return static_cast< O >( _value );
        }
    };
    
    
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
    
    
    template< typename T > class ratio
    {
    public:
        using value_type = T;
        
    protected:
        value_type _value;
        
    public:
        constexpr ratio() {}
        constexpr ratio( const T& v ) : _value{ v } {}
        // Implicit conversion from unit/unit of same unit
        template<
            template< typename > class Unit,
            typename O
        > constexpr ratio( const per< Unit, Unit, O >& o ) :
            _value{ static_cast< O >( o ) }
        {}
        
        // Ratio can be implicitly converted to its base type
        constexpr operator T () const
        {
            return _value;
        }
        template< typename O > explicit constexpr operator O () const
        {
            return static_cast< O >( _value );
        }
    };
} }


#endif
