#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_UTILS_HPP
#define JM_UNITS_CORE_INTERNAL_UTILS_HPP


#include <type_traits>  // enable_if, remove_cv, remove_reference


namespace JadeMatrix { namespace units { namespace internal // void_t //////////
{
    // `std::void_t` available in C++17
    template< typename... T > using void_t = void;
    
    // `std::remove_cvref_t` available in C++20
    template< typename T > using remove_cvref_t = typename std::remove_cv<
        typename std::remove_reference< T >::type
    >::type;
} } }


#endif
