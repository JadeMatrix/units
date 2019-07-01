#pragma once
#ifndef JM_UNITS_LINEAR_HPP
#define JM_UNITS_LINEAR_HPP


#include "core/define_unit.hpp"
#include "core/constants.hpp"


namespace JadeMatrix { namespace units
{
    struct          inch_traits {};
    struct          foot_traits {};
    struct          yard_traits {};
    struct          mile_traits {};
    struct         meter_traits {};
    struct        fathom_traits {};
    struct nautical_mile_traits {};
    
    DEFINE_ALL_PREFIXES_FOR_UNIT(          inches,          inch_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT(            feet,          foot_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT(           yards,          yard_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT(           miles,          mile_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT(          meters,         meter_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT(         fathoms,        fathom_traits )
    // FIXME: This will make the unit name string for `nautical_miles` be
    // prefixed with a space -- need more granular control in macro
    DEFINE_ALL_PREFIXES_FOR_UNIT( _nautical_miles, nautical_mile_traits )
    
    template< typename T > using nautical_miles = _nautical_miles< T >;
    template< typename T > using dozen_nautical_miles = dozen__nautical_miles< T >;
    
    DEFINE_ALL_STRINGS_FOR_UNIT(          inches,          "inches", "in" )
    DEFINE_ALL_STRINGS_FOR_UNIT(            feet,            "feet", "ft" )
    DEFINE_ALL_STRINGS_FOR_UNIT(           yards,           "yards", "yd" )
    DEFINE_ALL_STRINGS_FOR_UNIT(           miles,           "miles", "mi" )
    DEFINE_ALL_STRINGS_FOR_UNIT(          meters,          "meters",  "m" )
    DEFINE_ALL_STRINGS_FOR_UNIT(         fathoms,         "fathoms", "fm" )
    DEFINE_ALL_STRINGS_FOR_UNIT( _nautical_miles, " nautical miles",  "M" )
} }


namespace JadeMatrix { namespace units
{
    // Inches <-> X ////////////////////////////////////////////////////////////
    template< typename T > struct internal::traits_linear_relation<
        inch_traits,
        foot_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::foot_inches;
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
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
    template< typename T > struct internal::traits_linear_relation<
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
    template< typename T > struct internal::traits_linear_relation<
        inch_traits,
        meter_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::meter_cms;
        static constexpr T slope_den = constants< T >::inch_cms;
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
        inch_traits,
        fathom_traits,
        T
    >
    {
        static constexpr T slope_num = (
              constants< T >::foot_inches
            * constants< T >::yard_feet
            * constants< T >::fathom_yards
        );
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
        inch_traits,
        nautical_mile_traits,
        T
    >
    {
        static constexpr T slope_num = (
              constants< T >::meter_cms
            * constants< T >::nautical_mile_meters
        );
        static constexpr T slope_den = constants< T >::inch_cms;
        static constexpr T intercept = static_cast< T >( 0 );
    };
    
    // Feet <-> X //////////////////////////////////////////////////////////////
    template< typename T > struct internal::traits_linear_relation<
        foot_traits,
        yard_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::yard_feet;
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
        foot_traits,
        mile_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::mile_feet;
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
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
    template< typename T > struct internal::traits_linear_relation<
        foot_traits,
        fathom_traits,
        T
    >
    {
        static constexpr T slope_num = (
              constants< T >::yard_feet
            * constants< T >::fathom_yards
        );
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
        foot_traits,
        nautical_mile_traits,
        T
    >
    {
        static constexpr T slope_num = (
              constants< T >::meter_cms
            * constants< T >::nautical_mile_meters
        );
        static constexpr T slope_den = (
              constants< T >::inch_cms
            * constants< T >::foot_inches
        );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    
    // Yards <-> X /////////////////////////////////////////////////////////////
    template< typename T > struct internal::traits_linear_relation<
        yard_traits,
        mile_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::mile_feet;
        static constexpr T slope_den = constants< T >::yard_feet;
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
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
    template< typename T > struct internal::traits_linear_relation<
        yard_traits,
        fathom_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::fathom_yards;
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
        yard_traits,
        nautical_mile_traits,
        T
    >
    {
        static constexpr T slope_num = (
              constants< T >::meter_cms
            * constants< T >::nautical_mile_meters
        );
        static constexpr T slope_den = (
              constants< T >::inch_cms
            * constants< T >::foot_inches
            * constants< T >::yard_feet
        );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    
    // Miles <-> X /////////////////////////////////////////////////////////////
    template< typename T > struct internal::traits_linear_relation<
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
    template< typename T > struct internal::traits_linear_relation<
        mile_traits,
        fathom_traits,
        T
    >
    {
        static constexpr T slope_num = (
              constants< T >::yard_feet
            * constants< T >::fathom_yards
        );
        static constexpr T slope_den = constants< T >::mile_feet;
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
        mile_traits,
        nautical_mile_traits,
        T
    >
    {
        static constexpr T slope_num = (
              constants< T >::meter_cms
            * constants< T >::nautical_mile_meters
        );
        static constexpr T slope_den = (
              constants< T >::inch_cms
            * constants< T >::foot_inches
            * constants< T >::yard_feet
            * constants< T >::mile_feet
        );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    
    // Meters <-> X ////////////////////////////////////////////////////////////
    template< typename T > struct internal::traits_linear_relation<
        meter_traits,
        fathom_traits,
        T
    >
    {
        static constexpr T slope_num = (
              constants< T >::inch_cms
            * constants< T >::foot_inches
            * constants< T >::yard_feet
            * constants< T >::fathom_yards
        );
        static constexpr T slope_den = constants< T >::meter_cms;
        static constexpr T intercept = static_cast< T >( 0 );
    };
    template< typename T > struct internal::traits_linear_relation<
        meter_traits,
        nautical_mile_traits,
        T
    >
    {
        static constexpr T slope_num = constants< T >::nautical_mile_meters;
        static constexpr T slope_den = static_cast< T >( 1 );
        static constexpr T intercept = static_cast< T >( 0 );
    };
    
    // Fathoms <-> X ///////////////////////////////////////////////////////////
    template< typename T > struct internal::traits_linear_relation<
        fathom_traits,
        nautical_mile_traits,
        T
    >
    {
        static constexpr T slope_num = (
              constants< T >::meter_cms
            * constants< T >::nautical_mile_meters
        );
        static constexpr T slope_den = (
              constants< T >::inch_cms
            * constants< T >::foot_inches
            * constants< T >::yard_feet
            * constants< T >::fathom_yards
        );
        static constexpr T intercept = static_cast< T >( 0 );
    };
} }


#endif
