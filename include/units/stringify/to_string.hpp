#pragma once
#ifndef JM_UNITS_STRINGIFY_TO_STRING_HPP
#define JM_UNITS_STRINGIFY_TO_STRING_HPP


#include "../core/strings.hpp"
#include "../core/internal/core_type_detection.hpp"
#include "../core/internal/core_types.hpp"

#include <string>       // to_string
#include <type_traits>  // enable_if


namespace JadeMatrix { namespace units // Stream formatting operators //////////
{
    template< typename Unit > auto to_string( const Unit& u ) ->
        typename std::enable_if< (
            internal::is_unit< Unit >::value
            && !internal::is_ratio< Unit >::value
        ), std::string >::type
    {
        using std::to_string;
        return (
            to_string( static_cast< typename Unit::value_type >( u ) )
            + unit_strings<
                typename Unit::template unit_type< void >
            >::symbol()
        );
    }
    
    template< typename T > std::string to_string( const ratio< T >& u )
    {
        using std::to_string;
        return to_string( static_cast< T >( u ) );
    }
} }


#endif
