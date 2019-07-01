#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_CORE_TYPE_DETECTION_HPP
#define JM_UNITS_CORE_INTERNAL_CORE_TYPE_DETECTION_HPP


#include "core_types.hpp"
#include "utils.hpp"        // void_t

#include <type_traits>      // true_type, false_type, enable_if


namespace JadeMatrix { namespace units { namespace internal // Type detection //
{
    template<
        typename T,
        typename = void
    > struct is_basic_unit : std::false_type {};
    template< typename T > struct is_basic_unit<
        T,
        void_t< unit<
            typename T::traits_type,
            typename T::scale_type,
            typename T::value_type
        > >
    > : std::true_type {};
    
    template<
        typename T,
        typename = void
    > struct is_per : std::false_type {};
    template< typename T > struct is_per<
        T,
        void_t< per<
            T::template numer_unit,
            T::template denom_unit,
            typename T::value_type
        > >
    > : std::true_type {};
    
    template<
        typename T,
        typename = void
    > struct is_by : std::false_type {};
    template< typename T > struct is_by<
        T,
        void_t< by<
            T::template first_unit,
            T::template second_unit,
            typename T::value_type
        > >
    > : std::true_type {};
    
    template<
        typename T,
        typename = void
    > struct is_ratio : std::false_type {};
    template< typename T > struct is_ratio<
        T,
        typename std::enable_if< std::is_same<
            T,
            ratio< typename T::value_type >
        >::value >::type
    > : std::true_type {};
    
    template<
        typename T,
        typename = void
    > struct is_unit : std::false_type {};
    template< typename T > struct is_unit<
        T,
        typename std::enable_if< (
               is_basic_unit< T >::value
            || is_per       < T >::value
            || is_by        < T >::value
            || is_ratio     < T >::value
        ) >::type
    > : std::true_type {};
} } }


#endif
