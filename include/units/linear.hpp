#pragma once
#ifndef JM_UNITS_LINEAR_HPP
#define JM_UNITS_LINEAR_HPP


#include "core/constants.hpp"
#include "core/define_unit.hpp"


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
    
    DEFINE_ALL_STRINGS_FOR_UNIT(          inch_traits,          "inches", "in" )
    DEFINE_ALL_STRINGS_FOR_UNIT(          foot_traits,            "feet", "ft" )
    DEFINE_ALL_STRINGS_FOR_UNIT(          yard_traits,           "yards", "yd" )
    DEFINE_ALL_STRINGS_FOR_UNIT(          mile_traits,           "miles", "mi" )
    DEFINE_ALL_STRINGS_FOR_UNIT(         meter_traits,          "meters",  "m" )
    DEFINE_ALL_STRINGS_FOR_UNIT(        fathom_traits,         "fathoms", "fm" )
    DEFINE_ALL_STRINGS_FOR_UNIT( nautical_mile_traits, " nautical miles",  "M" )
} }


namespace JadeMatrix { namespace units
{
    // Inches <-> X ////////////////////////////////////////////////////////////
    
    struct inches_feet_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = constants::foot_inches< T >::value;
            static constexpr unsigned char slope_den = /*static_cast< T >*/( 1 );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    inches_feet_linear_relation units_linear_relation_lookup(
        inch_traits&&,
        foot_traits&&
    );
    
    struct inches_yards_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                  constants::foot_inches< T >::value
                * constants::yard_feet< T >::value
            );
            static constexpr unsigned char slope_den = /*static_cast< T >*/( 1 );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    inches_yards_linear_relation units_linear_relation_lookup(
        inch_traits&&,
        yard_traits&&
    );
    
    struct inches_miles_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                  constants::foot_inches< T >::value
                * constants::mile_feet< T >::value
            );
            static constexpr unsigned char slope_den = /*static_cast< T >*/( 1 );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    inches_miles_linear_relation units_linear_relation_lookup(
        inch_traits&&,
        mile_traits&&
    );
    
    struct inches_meters_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = constants::meter_cms< T >::value;
            static constexpr auto slope_den = constants::inch_cms< T >::value;
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    inches_meters_linear_relation units_linear_relation_lookup(
        inch_traits&&,
        meter_traits&&
    );
    
    struct inches_fathoms_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                  constants::foot_inches< T >::value
                * constants::yard_feet< T >::value
                * constants::fathom_yards< T >::value
            );
            static constexpr unsigned char slope_den = /*static_cast< T >*/( 1 );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    inches_fathoms_linear_relation units_linear_relation_lookup(
        inch_traits&&,
        fathom_traits&&
    );
    
    struct inches_nautical_miles_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                  constants::meter_cms< T >::value
                * constants::nautical_mile_meters< T >::value
            );
            static constexpr auto slope_den = constants::inch_cms< T >::value;
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    inches_nautical_miles_linear_relation units_linear_relation_lookup(
        inch_traits&&,
        nautical_mile_traits&&
    );
    
    // Feet <-> X //////////////////////////////////////////////////////////////
    
    struct feet_yards_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = constants::yard_feet< T >::value;
            static constexpr unsigned char slope_den = /*static_cast< T >*/( 1 );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    feet_yards_linear_relation units_linear_relation_lookup(
        foot_traits&&,
        yard_traits&&
    );
    
    struct feet_miles_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = constants::mile_feet< T >::value;
            static constexpr unsigned char slope_den = /*static_cast< T >*/( 1 );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    feet_miles_linear_relation units_linear_relation_lookup(
        foot_traits&&,
        mile_traits&&
    );
    
    struct feet_meters_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = constants::meter_cms< T >::value;
            static constexpr auto slope_den = (
                  constants::inch_cms< T >::value
                * constants::foot_inches< T >::value
            );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    feet_meters_linear_relation units_linear_relation_lookup(
        foot_traits&&,
        meter_traits&&
    );
    
    struct feet_fathoms_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                  constants::yard_feet< T >::value
                * constants::fathom_yards< T >::value
            );
            static constexpr unsigned char slope_den = /*static_cast< T >*/( 1 );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    feet_fathoms_linear_relation units_linear_relation_lookup(
        foot_traits&&,
        fathom_traits&&
    );
    
    struct feet_nautical_miles_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                  constants::meter_cms< T >::value
                * constants::nautical_mile_meters< T >::value
            );
            static constexpr auto slope_den = (
                  constants::inch_cms< T >::value
                * constants::foot_inches< T >::value
            );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    feet_nautical_miles_linear_relation units_linear_relation_lookup(
        foot_traits&&,
        nautical_mile_traits&&
    );
    
    // Yards <-> X /////////////////////////////////////////////////////////////
    
    struct yards_miles_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = constants::mile_feet< T >::value;
            static constexpr auto slope_den = constants::yard_feet< T >::value;
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    yards_miles_linear_relation units_linear_relation_lookup(
        yard_traits&&,
        mile_traits&&
    );
    
    struct yards_meters_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = constants::meter_cms< T >::value;
            static constexpr auto slope_den = (
                  constants::inch_cms< T >::value
                * constants::foot_inches< T >::value
                * constants::yard_feet< T >::value
            );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    yards_meters_linear_relation units_linear_relation_lookup(
        yard_traits&&,
        meter_traits&&
    );
    
    struct yards_fathoms_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = constants::fathom_yards< T >::value;
            static constexpr unsigned char slope_den = /*static_cast< T >*/( 1 );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    yards_fathoms_linear_relation units_linear_relation_lookup(
        yard_traits&&,
        fathom_traits&&
    );
    
    struct yards_nautical_miles_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                  constants::meter_cms< T >::value
                * constants::nautical_mile_meters< T >::value
            );
            static constexpr auto slope_den = (
                  constants::inch_cms< T >::value
                * constants::foot_inches< T >::value
                * constants::yard_feet< T >::value
            );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    yards_nautical_miles_linear_relation units_linear_relation_lookup(
        yard_traits&&,
        nautical_mile_traits&&
    );
    
    // Miles <-> X /////////////////////////////////////////////////////////////
    
    struct miles_meters_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = constants::meter_cms< T >::value;
            static constexpr auto slope_den = (
                  constants::inch_cms< T >::value
                * constants::foot_inches< T >::value
                * constants::mile_feet< T >::value
            );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    miles_meters_linear_relation units_linear_relation_lookup(
        mile_traits&&,
        meter_traits&&
    );
    
    struct miles_fathoms_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                  constants::yard_feet< T >::value
                * constants::fathom_yards< T >::value
            );
            static constexpr auto slope_den = constants::mile_feet< T >::value;
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    miles_fathoms_linear_relation units_linear_relation_lookup(
        mile_traits&&,
        fathom_traits&&
    );
    
    struct miles_nautical_miles_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                  constants::meter_cms< T >::value
                * constants::nautical_mile_meters< T >::value
            );
            static constexpr auto slope_den = (
                  constants::inch_cms< T >::value
                * constants::foot_inches< T >::value
                * constants::yard_feet< T >::value
                * constants::mile_feet< T >::value
            );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    miles_nautical_miles_linear_relation units_linear_relation_lookup(
        mile_traits&&,
        nautical_mile_traits&&
    );
    
    // Meters <-> X ////////////////////////////////////////////////////////////
    
    struct meters_fathoms_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                  constants::inch_cms< T >::value
                * constants::foot_inches< T >::value
                * constants::yard_feet< T >::value
                * constants::fathom_yards< T >::value
            );
            static constexpr auto slope_den = constants::meter_cms< T >::value;
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    meters_fathoms_linear_relation units_linear_relation_lookup(
        meter_traits&&,
        fathom_traits&&
    );
    
    struct meters_nautical_miles_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = constants::nautical_mile_meters< T >::value;
            static constexpr unsigned char slope_den = /*static_cast< T >*/( 1 );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    meters_nautical_miles_linear_relation units_linear_relation_lookup(
        meter_traits&&,
        nautical_mile_traits&&
    );
    
    // Fathoms <-> X ///////////////////////////////////////////////////////////
    
    struct fathoms_nautical_miles_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                  constants::meter_cms< T >::value
                * constants::nautical_mile_meters< T >::value
            );
            static constexpr auto slope_den = (
                  constants::inch_cms< T >::value
                * constants::foot_inches< T >::value
                * constants::yard_feet< T >::value
                * constants::fathom_yards< T >::value
            );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    fathoms_nautical_miles_linear_relation units_linear_relation_lookup(
        fathom_traits&&,
        nautical_mile_traits&&
    );
} }


#endif
