#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_UTILS_HPP
#define JM_UNITS_CORE_INTERNAL_UTILS_HPP


#include <type_traits>


namespace JadeMatrix { namespace units { namespace internal
{
    // `std::void_t` available in C++17
    template< typename... T > using void_t = void;
} } }


namespace JadeMatrix { namespace units { namespace internal // If/else /////////
{
    // If `Condition` is `true`, member `type` is an alias to type `A`;
    // otherwise it is an alias to type `B`.  Note that both `A` and `B` need to
    // be valid types; this just permits a simple switch between the two.
    
    template<
        bool Condition,
        typename A,
        typename B,
        typename = void
    > struct if_else;
    
    template<
        bool Condition,
        typename A,
        typename B
    > struct if_else<
        Condition,
        A,
        B,
        typename std::enable_if< Condition >::type
    >
    {
        using type = A;
    };
    
    template<
        bool Condition,
        typename A,
        typename B
    > struct if_else<
        Condition,
        A,
        B,
        typename std::enable_if< !Condition >::type
    >
    {
        using type = B;
    };
} } }


#endif
