#pragma once
#ifndef JM_UNITS_CORE_UNIT_HPP
#define JM_UNITS_CORE_UNIT_HPP


#include "internal/convert.hpp"
#include "internal/core_type_detection.hpp"
#include "internal/core_types.hpp"
#include "internal/reduce.hpp"

#include <type_traits>  // enable_if, is_same


namespace JadeMatrix { namespace units // Basic unit class /////////////////////
{
    template<
        typename Traits,
        typename Scale,
        typename T
    > class unit
    {
    public:
        using traits_type = Traits;
        using  scale_type = typename Scale::type;
        using  value_type = T;
        template< typename O > using unit_type = unit<
            traits_type,
            scale_type,
            O
        >;
        
    protected:
        value_type _value;
        
        template<
            template< typename > class OtherUnit
        > using convert_from = internal::conversion< unit_type, OtherUnit >;
        
    public:
        constexpr unit() {}
        constexpr unit( const value_type& v ) : _value{ v } {}
        // Copy/move constructors defaulted mostly for documentation purposes
        constexpr unit( const unit&  ) = default;
        constexpr unit(       unit&& ) = default;
        
        // Implicit conversion from convertible units of the same storage type
        template<
            typename O,
            typename = typename std::enable_if<
                convert_from< O::template unit_type >::exists
                && std::is_same< value_type, typename O::value_type >::value
            >::type
        > constexpr unit( const O& o ) :
            _value{ convert_from< O::template unit_type >::apply( o ) }
        {}
        // Explicit conversion from convertible units of a different storage
        // type; two 'enable' template parameters so its template signature is
        // different than the implicit version
        template<
            typename O,
            typename = typename std::enable_if<
                convert_from< O::template unit_type >::exists
            >::type,
            typename = typename std::enable_if<
                !std::is_same< value_type, typename O::value_type >::value
            >::type
        > explicit constexpr unit( const O& o ) :
            _value{ static_cast< value_type >(
                convert_from< O::template unit_type >::apply( o )
            ) }
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
        typename Traits,
        typename Scale
    > class unit< Traits, Scale, void >
    {
    public:
        using traits_type = Traits;
        using  scale_type = typename Scale::type;
        using  value_type = void;
        template< typename O > using unit_type = unit<
            traits_type,
            scale_type,
            O
        >;
        
        unit() = delete;
        ~unit() = delete;
    };
} }


#endif
