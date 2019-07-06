#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_CORE_TYPE_DETECTION_HPP
#define JM_UNITS_CORE_INTERNAL_CORE_TYPE_DETECTION_HPP


#include "core_types.hpp"
#include "utils.hpp"    // void_t

#include <type_traits>  // true_type, false_type, enable_if
#include <ratio>        // ratio_equal


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


namespace JadeMatrix { namespace units { namespace internal // Type comparison /
{
    // A way to check if two units are the same, as using `std::is_same` with
    // the `void` specializations of unit template types is unreliable
    
    template<
        template< typename > class  FirstUnit,
        template< typename > class SecondUnit,
        typename = void
    > struct is_same_unit : std::false_type {};
    
    template<
        template< typename > class  FirstUnit,
        template< typename > class SecondUnit
    > struct is_same_unit<
         FirstUnit,
        SecondUnit,
        typename std::enable_if<
               is_ratio<  FirstUnit< void > >::value
            && is_ratio< SecondUnit< void > >::value
        >::type
    > : std::true_type {};
    
    template<
        template< typename > class  FirstUnit,
        template< typename > class SecondUnit
    > struct is_same_unit<
         FirstUnit,
        SecondUnit,
        typename std::enable_if<
               is_basic_unit<  FirstUnit< void > >::value
            && is_basic_unit< SecondUnit< void > >::value
            && std::is_same<
                typename  FirstUnit< void >::traits_type,
                typename SecondUnit< void >::traits_type
            >::value
            && std::ratio_equal<
                typename  FirstUnit< void >::scale_type,
                typename SecondUnit< void >::scale_type
            >::value
        >::type
    > : std::true_type {};
    
    template<
        template< typename > class  FirstUnit,
        template< typename > class SecondUnit
    > struct is_same_unit<
         FirstUnit,
        SecondUnit,
        typename std::enable_if<
               is_per<  FirstUnit< void > >::value
            && is_per< SecondUnit< void > >::value
            && is_same_unit<
                 FirstUnit< void >::template numer_unit,
                SecondUnit< void >::template numer_unit
            >::value
            && is_same_unit<
                 FirstUnit< void >::template denom_unit,
                SecondUnit< void >::template denom_unit
            >::value
        >::type
    > : std::true_type {};
    
    template<
        template< typename > class  FirstUnit,
        template< typename > class SecondUnit
    > struct is_same_unit<
         FirstUnit,
        SecondUnit,
        typename std::enable_if<
               is_by<  FirstUnit< void > >::value
            && is_by< SecondUnit< void > >::value
            && is_same_unit<
                 FirstUnit< void >::template first_unit,
                SecondUnit< void >::template first_unit
            >::value
            && is_same_unit<
                 FirstUnit< void >::template second_unit,
                SecondUnit< void >::template second_unit
            >::value
        >::type
    > : std::true_type {};
} } }


#endif
