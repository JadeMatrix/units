#pragma once
#ifndef JM_UNITS_CORE_STREAM_FORMAT_HPP
#define JM_UNITS_CORE_STREAM_FORMAT_HPP


#include "strings.hpp"
#include "internal/core_type_detection.hpp"
#include "internal/core_types.hpp"

#include <ostream>
#include <type_traits>  // enable_if


namespace JadeMatrix { namespace units // Stream formatting operators //////////
{
    template< typename Unit > auto operator<<(
        std::ostream& out,
        const Unit& u
    ) -> typename std::enable_if< (
        internal::is_unit< Unit >::value
        && !internal::is_ratio< Unit >::value
    ), std::ostream& >::type
    {
        std::ostream::sentry s{ out };
        if( s ) out
            << static_cast< typename Unit::value_type >( u )
            << unit_strings< Unit >::symbol()
        ;
        return out;
    }
    
    template< typename T > std::ostream& operator<<(
        std::ostream& out,
        const ratio< T >& u
    )
    {
        std::ostream::sentry s{ out };
        if( s ) out << static_cast< T >( u );
        return out;
    }
} }


#endif
