#pragma once
#ifndef JM_INTERNAL_UNITS_UNIT_HPP
#define JM_INTERNAL_UNITS_UNIT_HPP


#include "core_types.hpp"
#include "traits_utils.hpp"

#include <type_traits>


namespace JadeMatrix { namespace units // Basic unit class /////////////////////
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
        > static constexpr value_type from(
            const unit< Other_Traits, O, Other_Scale >& o
        )
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
} }


#endif
