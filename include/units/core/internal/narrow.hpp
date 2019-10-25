#pragma once
#ifndef JM_UNITS_INTERNAL_NARROW_HPP
#define JM_UNITS_INTERNAL_NARROW_HPP


#include <type_traits>  // enable_if, is_floating_point


namespace JadeMatrix { namespace units { namespace internal
{
    // Explicit floating point narrowing logic so as not to trigger double
    // promotion warnings
    
    template<
        typename To,
        typename From,
        typename = void
    > struct narrow { using type = From; };
    template< typename To > struct narrow<
        To,
        long double,
        typename std::enable_if< std::is_floating_point< To >::value >::type
    > { using type = To; };
} } }


#endif
