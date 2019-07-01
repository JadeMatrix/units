#pragma once
#ifndef JM_UNITS_ANGULAR_HPP
#define JM_UNITS_ANGULAR_HPP


#include "core/define_unit.hpp"
#include "core/constants.hpp"


namespace JadeMatrix { namespace units // Declarations /////////////////////////
{
    struct     degree_traits {};
    struct     radian_traits {};
    struct  arcminute_traits {};
    struct  arcsecond_traits {};
    struct revolution_traits {};
    
    DEFINE_ALL_PREFIXES_FOR_UNIT(     degrees,     degree_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT(     radians,     radian_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT(  arcminutes,  arcminute_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT(  arcseconds,  arcsecond_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT( revolutions, revolution_traits )
    
    DEFINE_ALL_STRINGS_FOR_UNIT(     degree_traits,     "degrees",   "°" )
    DEFINE_ALL_STRINGS_FOR_UNIT(     radian_traits,     "radians",   "r" )
    DEFINE_ALL_STRINGS_FOR_UNIT(  arcminute_traits,  "arcminutes", "moa" )
    DEFINE_ALL_STRINGS_FOR_UNIT(  arcsecond_traits,  "arcseconds", "soa" )
    DEFINE_ALL_STRINGS_FOR_UNIT( revolution_traits, "revolutions", "rev" )
} }


namespace JadeMatrix { namespace units
{
    // Degrees <-> X ///////////////////////////////////////////////////////////
    template< typename T > struct internal::traits_linear_relation<
        degree_traits,
        radian_traits,
        T
    >
    {
        static constexpr T slope_num = (
            constants< T >::circle_degrees
            / static_cast< T >( 2 )
        );
        static constexpr T slope_den = constants< T >::pi;
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
        degree_traits,
        arcminute_traits,
        T
    >
    {
        static constexpr T slope_num = static_cast< T >( 1 );
        static constexpr T slope_den = constants< T >::hour_minutes;
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
        degree_traits,
        arcsecond_traits,
        T
    >
    {
        static constexpr T slope_num = static_cast< T >( 1 );
        static constexpr T slope_den = (
              constants< T >::hour_minutes
            * constants< T >::minute_seconds
        );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
        degree_traits,
        revolution_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::circle_degrees;
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    
    // Radians <-> X ///////////////////////////////////////////////////////////
    template< typename T > struct internal::traits_linear_relation<
        radian_traits,
        arcminute_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::pi * 2;
        static constexpr T slope_den = (
              constants< T >::circle_degrees
            * constants< T >::hour_minutes
        );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
        radian_traits,
        arcsecond_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::pi * 2;
        static constexpr T slope_den = (
              constants< T >::circle_degrees
            * constants< T >::hour_minutes
            * constants< T >::minute_seconds
        );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
        radian_traits,
        revolution_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::pi * 2;
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    
    // Arcminutes <-> X ////////////////////////////////////////////////////////
    template< typename T > struct internal::traits_linear_relation<
        arcminute_traits,
        arcsecond_traits,
        T
    >
    {
        static constexpr T slope_num = static_cast< T >( 1 );
        static constexpr T slope_den = constants< T >::minute_seconds;
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
        arcminute_traits,
        revolution_traits,
        T
    >
    {
        static constexpr T slope_num = (
              constants< T >::circle_degrees
            * constants< T >::hour_minutes
        );
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    
    // Arcseconds <-> X ////////////////////////////////////////////////////////
    template< typename T > struct internal::traits_linear_relation<
        arcsecond_traits,
        revolution_traits,
        T
    >
    {
        static constexpr T slope_num = (
              constants< T >::circle_degrees
            * constants< T >::hour_minutes
            * constants< T >::minute_seconds
        );
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
} }


#endif
