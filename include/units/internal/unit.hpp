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
            return convert::template from< Other_Traits >(
                static_cast< O >( o )
            ) * scale_type::scale( Other_Scale::unscale( 1 ) );
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
        template< typename O > constexpr unit(
            const typename std::enable_if< (
                !is_unit< O >::value
                && !std::is_arithmetic< O >::value
            ), O
        >::value& v ) : _value{ traits_type::from( v ) }
        {}
        
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
