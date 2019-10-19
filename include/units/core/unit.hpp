#pragma once
#ifndef JM_UNITS_CORE_UNIT_HPP
#define JM_UNITS_CORE_UNIT_HPP


#include "internal/conversion.hpp"
#include "internal/core_type_detection.hpp"
#include "internal/core_types.hpp"

#include <type_traits>  // enable_if, is_same


namespace JadeMatrix { namespace units // Basic unit class /////////////////////
{
    namespace internal
    {
        template<
            typename Traits,
            typename Scale,
            typename T
        > class unit_impl
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
            constexpr unit_impl() {}
            constexpr unit_impl( const value_type& v ) : _value{ v } {}
            
            template<
                typename O,
                typename = typename std::enable_if<
                    convert_from< O::template unit_type >::exists
                >::type
            > constexpr unit_impl( const O& o ) :
                _value( convert_from< O::template unit_type >::apply( o ) )
            {}
        };
        
        template<
            typename Traits,
            typename Scale
        > class unit_impl< Traits, Scale, void >
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
            
            unit_impl() = delete;
            ~unit_impl() = delete;
        };
    }
    
    template<
        typename Traits,
        typename Scale,
        typename T
    > class unit : public internal::unit_impl< Traits, Scale, T >
    {
    public:
        using internal::unit_impl< Traits, Scale, T >::unit_impl;
        
        template<
            typename O,
            typename = typename std::enable_if<
                !internal::is_unit< O >::value
            >::type
        > explicit constexpr operator O () const
        {
            return static_cast< O >(
                internal::unit_impl< Traits, Scale, T >::_value
            );
        }
    };
} }


#endif
