#pragma once
#ifndef JM_UNITS_LINEAR_HPP
#define JM_UNITS_LINEAR_HPP


#include "units.hpp"
#include "scales.hpp"
#include "strings.hpp"
#include "internal/constants.hpp"


namespace JadeMatrix { namespace units
{
    struct  inch_traits {};
    struct  foot_traits {};
    struct  yard_traits {};
    struct  mile_traits {};
    struct meter_traits {};
    
    DEFINE_ALL_PREFIXES_FOR_UNIT( inches,  inch_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT(   feet,  foot_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT(  yards,  yard_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT(  miles,  mile_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT( meters, meter_traits )
    
    DEFINE_ALL_STRINGS_FOR_UNIT( inches, "inches", "in" )
    DEFINE_ALL_STRINGS_FOR_UNIT(   feet,   "feet", "ft" )
    DEFINE_ALL_STRINGS_FOR_UNIT(  yards,  "yards", "yd" )
    DEFINE_ALL_STRINGS_FOR_UNIT(  miles,  "miles", "mi" )
    DEFINE_ALL_STRINGS_FOR_UNIT( meters, "meters",  "m" )
} }


namespace JadeMatrix { namespace units
{
    template< typename T > struct traits_linear_relation<
        inch_traits,
        foot_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::foot_inches;
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct traits_linear_relation<
        inch_traits,
        yard_traits,
        T
    >
    {
        static constexpr T slope_num = (
              constants< T >::foot_inches
            * constants< T >::yard_feet
        );
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct traits_linear_relation<
        inch_traits,
        mile_traits,
        T
    >
    {
        static constexpr T slope_num = (
              constants< T >::foot_inches
            * constants< T >::mile_feet
        );
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct traits_linear_relation<
        inch_traits,
        meter_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::meter_cms;
        static constexpr T slope_den = constants< T >::inch_cms;
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct traits_linear_relation<
        foot_traits,
        yard_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::yard_feet;
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct traits_linear_relation<
        foot_traits,
        mile_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::mile_feet;
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct traits_linear_relation<
        foot_traits,
        meter_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::meter_cms;
        static constexpr T slope_den = (
              constants< T >::inch_cms
            * constants< T >::foot_inches
        );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct traits_linear_relation<
        yard_traits,
        mile_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::mile_feet;
        static constexpr T slope_den = constants< T >::yard_feet;
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct traits_linear_relation<
        yard_traits,
        meter_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::meter_cms;
        static constexpr T slope_den = (
              constants< T >::inch_cms
            * constants< T >::foot_inches
            * constants< T >::yard_feet
        );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct traits_linear_relation<
        mile_traits,
        meter_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::meter_cms;
        static constexpr T slope_den = (
              constants< T >::inch_cms
            * constants< T >::foot_inches
            * constants< T >::mile_feet
        );
        static constexpr T intercept = static_cast< T >( 0 );
    };
} }


#endif
