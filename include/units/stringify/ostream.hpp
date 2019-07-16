#pragma once
#ifndef JM_UNITS_STRINGIFY_OSTREAM_HPP
#define JM_UNITS_STRINGIFY_OSTREAM_HPP


#include "to_string.hpp"
#include "../core/internal/utils.hpp"   // void_t
#include "../core/strings.hpp"

#include <ostream>
#include <type_traits>  // enable_if, true_type, false_type


namespace JadeMatrix { namespace units // Stream format operator ///////////////
{
    namespace internal
    {
        template<
            typename T,
            typename = void
        > struct has_to_string : std::false_type {};
        template< typename T > struct has_to_string<
            T,
            void_t< decltype( to_string( std::declval< T >() ) ) >
        > : std::false_type {};
    }
    
    template< typename Unit > auto operator<<(
        std::ostream& out,
        const Unit& u
    ) -> typename std::enable_if< (
           internal::is_unit< Unit >::value
        && internal::has_to_string< typename Unit::value_type >::value
    ), std::ostream& >::type
    {
        std::ostream::sentry s{ out };
        if( s ) out << to_string( u );
        return out;
    }
    
    template< typename Unit > auto operator<<(
        std::ostream& out,
        const Unit& u
    ) -> typename std::enable_if< (
            internal::is_unit< Unit >::value
        && !internal::has_to_string< typename Unit::value_type >::value
    ), std::ostream& >::type
    {
        std::ostream::sentry s{ out };
        if( s ) out
            << static_cast< typename Unit::value_type >( u )
            << unit_strings< Unit >::symbol()
        ;
        return out;
    }
} }


#endif
