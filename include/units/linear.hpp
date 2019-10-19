#pragma once
#ifndef JM_UNITS_LINEAR_HPP
#define JM_UNITS_LINEAR_HPP


#include "core/constants.hpp"
#include "core/define_unit.hpp"
#include "temporal.hpp"


namespace JadeMatrix { namespace units
{
    struct          inch_traits {};
    struct          foot_traits {};
    struct          yard_traits {};
    struct          mile_traits {};
    struct         meter_traits {};
    struct        fathom_traits {};
    struct nautical_mile_traits {};
    
    
    #define DEFINE_PREFIX_FOR_inches(         PREFIX, SCALE ) template< typename T > using PREFIX##inches          = unit<          inch_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_feet(           PREFIX, SCALE ) template< typename T > using PREFIX##feet            = unit<          foot_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_yards(          PREFIX, SCALE ) template< typename T > using PREFIX##yards           = unit<          yard_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_miles(          PREFIX, SCALE ) template< typename T > using PREFIX##miles           = unit<          mile_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_meters(         PREFIX, SCALE ) template< typename T > using PREFIX##meters          = unit<         meter_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_fathoms(        PREFIX, SCALE ) template< typename T > using PREFIX##fathoms         = unit<        fathom_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_nautical_miles( PREFIX, SCALE ) template< typename T > using PREFIX##_nautical_miles = unit< nautical_mile_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_knots(          PREFIX, SCALE ) template< typename T > using PREFIX##knots           = per< PREFIX##_nautical_miles, hours, T >;
    
    // Define unit & underscored prefixes separately for nautical miles so as
    // not to add excess underscores (and knots as they rely on those aliases)
    template< typename T > using       nautical_miles = unit< nautical_mile_traits,  unit_scale, T >;
    template< typename T > using dozen_nautical_miles = unit< nautical_mile_traits, dozen_scale, T >;
    JM_UNITS_INTERNAL_FOREACH_SCALE_MOST( DEFINE_PREFIX_FOR_nautical_miles )
    JM_UNITS_INTERNAL_FOREACH_SCALE_YT  ( DEFINE_PREFIX_FOR_nautical_miles )
    JM_UNITS_INTERNAL_FOREACH_SCALE_ZT  ( DEFINE_PREFIX_FOR_nautical_miles )
    template< typename T > using       knots = per<       nautical_miles, hours, T >;
    template< typename T > using dozen_knots = per< dozen_nautical_miles, hours, T >;
    JM_UNITS_INTERNAL_FOREACH_SCALE_MOST( DEFINE_PREFIX_FOR_knots )
    JM_UNITS_INTERNAL_FOREACH_SCALE_YT  ( DEFINE_PREFIX_FOR_knots )
    JM_UNITS_INTERNAL_FOREACH_SCALE_ZT  ( DEFINE_PREFIX_FOR_knots )
    
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_inches  )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_feet    )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_yards   )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_miles   )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_meters  )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_fathoms )
    
    #undef DEFINE_PREFIX_FOR_inches
    #undef DEFINE_PREFIX_FOR_feet
    #undef DEFINE_PREFIX_FOR_yards
    #undef DEFINE_PREFIX_FOR_miles
    #undef DEFINE_PREFIX_FOR_meters
    #undef DEFINE_PREFIX_FOR_fathoms
    #undef DEFINE_PREFIX_FOR_nautical_miles
    #undef DEFINE_PREFIX_FOR_knots
    
    
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS(          inch_traits,          "inches", "in" )
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS(          foot_traits,            "feet", "ft" )
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS(          yard_traits,           "yards", "yd" )
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS(          mile_traits,           "miles", "mi" )
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS(         meter_traits,          "meters",  "m" )
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS(        fathom_traits,         "fathoms", "fm" )
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS( nautical_mile_traits, " nautical miles",  "M" )
    
    template<> struct unit_strings< knots< void > >
    {
        static const std::string& name()
        {
            static const std::string s{ "knots" };
            return s;
        }
        static const std::string& symbol()
        {
            static const std::string s{ "kt" };
            return s;
        }
    };
} }


namespace JadeMatrix { namespace units
{
    // Inches <-> X ////////////////////////////////////////////////////////////
    
    struct inches_feet_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                constants::foot_inches< T >::value
            );
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
            static constexpr auto slope_num = (
                constants::fathom_yards< T >::value
            );
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
            static constexpr auto slope_num = (
                constants::nautical_mile_meters< T >::value
            );
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
        };
    };
    fathoms_nautical_miles_linear_relation units_linear_relation_lookup(
        fathom_traits&&,
        nautical_mile_traits&&
    );
} }


#endif
