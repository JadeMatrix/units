#pragma once
#ifndef JM_UNITS_CORE_STREAM_FORMAT_HPP
#define JM_UNITS_CORE_STREAM_FORMAT_HPP


#include "strings.hpp"
#include "internal/core_types.hpp"
#include "internal/traits_utils.hpp"

#include <ostream>


namespace JadeMatrix { namespace units // Stream formatting operators //////////
{
    template< typename Unit > auto operator<<(
        std::ostream& out,
        const Unit& u
    ) -> typename std::enable_if<
        ( is_unit< Unit >::value && !is_ratio< Unit >::value ),
        std::ostream&
    >::type
    {
        return out
            << static_cast< typename Unit::value_type >( u )
            << unit_strings< Unit >::symbol()
        ;
    }
    
    template< typename T > std::ostream& operator<<(
        std::ostream& out,
        const ratio< T >& u
    )
    {
        return out << static_cast< T >( u );
    }
} }


#endif
