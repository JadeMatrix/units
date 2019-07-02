#pragma once
#ifndef JM_UNITS_CORE_UNIT_HPP
#define JM_UNITS_CORE_UNIT_HPP


#include "internal/core_type_detection.hpp"
#include "internal/core_types.hpp"
#include "internal/linear_relation.hpp"
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
        
        using convert = internal::conversion< unit_type >;
        
    public:
        constexpr unit() {}
        constexpr unit( const value_type& v ) : _value{ v } {}
        template<
            typename O,
            typename = typename std::enable_if<
                internal::is_unit< O >::value
                && std::is_same< value_type, typename O::value_type >::value
            >::type
        > constexpr unit( const O& o ) :
            _value{ convert::from( o ) }
        {}
        // Two 'enable' template parameters so its template signature is
        // different than the implicit version
        template<
            typename O,
            typename = typename std::enable_if<
                internal::is_unit< O >::value
            >::type,
            typename = typename std::enable_if<
                !std::is_same< value_type, typename O::value_type >::value
            >::type
        > explicit constexpr unit( const O& o ) :
            _value{ static_cast< value_type >( convert::from( o ) ) }
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
