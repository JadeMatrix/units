#pragma once
#ifndef JM_UNITS_ANGULAR_HPP
#define JM_UNITS_ANGULAR_HPP


#include "core/constants.hpp"
#include "core/define_unit.hpp"
#include "temporal.hpp"


namespace JadeMatrix { namespace units // Declarations /////////////////////////
{
    struct     degree_traits {};
    struct     radian_traits {};
    struct  arcminute_traits {};
    struct  arcsecond_traits {};
    struct revolution_traits {};
    
    
    #define DEFINE_PREFIX_FOR_degrees(     PREFIX, SCALE ) template< typename T > using PREFIX##degrees     = unit<     degree_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_radians(     PREFIX, SCALE ) template< typename T > using PREFIX##radians     = unit<     radian_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_arcminutes(  PREFIX, SCALE ) template< typename T > using PREFIX##arcminutes  = unit<  arcminute_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_arcseconds(  PREFIX, SCALE ) template< typename T > using PREFIX##arcseconds  = unit<  arcsecond_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_revolutions( PREFIX, SCALE ) template< typename T > using PREFIX##revolutions = unit< revolution_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_rpm(         PREFIX, SCALE ) template< typename T > using PREFIX##rpm         = per< PREFIX##revolutions, minutes, T >;
    
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_degrees     )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_radians     )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_arcminutes  )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_arcseconds  )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_revolutions )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_rpm         )
    
    #undef DEFINE_PREFIX_FOR_degrees
    #undef DEFINE_PREFIX_FOR_radians
    #undef DEFINE_PREFIX_FOR_arcminutes
    #undef DEFINE_PREFIX_FOR_arcseconds
    #undef DEFINE_PREFIX_FOR_revolutions
    #undef DEFINE_PREFIX_FOR_rpm
    
    
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS(     degree_traits,     "degrees",   "°" )
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS(     radian_traits,     "radians",   "r" )
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS(  arcminute_traits,  "arcminutes", "moa" )
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS(  arcsecond_traits,  "arcseconds", "soa" )
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS( revolution_traits, "revolutions", "rev" )
    
    template<> struct unit_strings< rpm< void > >
    {
        static const std::string& name()
        {
            return unit_strings< rpm< void > >::name();
        }
        static const std::string& symbol()
        {
            static const std::string s{ "rpm" };
            return s;
        }
    };
} }


namespace JadeMatrix { namespace units
{
    // Degrees <-> X ///////////////////////////////////////////////////////////
    
    struct degrees_radians_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                constants::circle_degrees< T >::value / 2
            );
            static constexpr auto slope_den = constants::pi< T >::value;
        };
    };
    degrees_radians_linear_relation units_linear_relation_lookup(
        degree_traits&&,
        radian_traits&&
    );
    
    struct degrees_arcminutes_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_den = (
                constants::hour_minutes< T >::value
            );
        };
    };
    degrees_arcminutes_linear_relation units_linear_relation_lookup(
        degree_traits&&,
        arcminute_traits&&
    );
    
    struct degrees_arcseconds_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_den = (
                  constants::hour_minutes< T >::value
                * constants::minute_seconds< T >::value
            );
        };
    };
    degrees_arcseconds_linear_relation units_linear_relation_lookup(
        degree_traits&&,
        arcsecond_traits&&
    );
    
    struct degrees_revolutions_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                constants::circle_degrees< T >::value
            );
        };
    };
    degrees_revolutions_linear_relation units_linear_relation_lookup(
        degree_traits&&,
        revolution_traits&&
    );
    
    // Radians <-> X ///////////////////////////////////////////////////////////
    
    struct radians_arcminutes_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = constants::pi< T >::value * 2;
            static constexpr auto slope_den = (
                  constants::circle_degrees< T >::value
                * constants::hour_minutes< T >::value
            );
        };
    };
    radians_arcminutes_linear_relation units_linear_relation_lookup(
        radian_traits&&,
        arcminute_traits&&
    );
    
    struct radians_arcseconds_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = constants::pi< T >::value * 2;
            static constexpr auto slope_den = (
                  constants::circle_degrees< T >::value
                * constants::hour_minutes< T >::value
                * constants::minute_seconds< T >::value
            );
        };
    };
    radians_arcseconds_linear_relation units_linear_relation_lookup(
        radian_traits&&,
        arcsecond_traits&&
    );
    
    struct radians_revolutions_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = constants::pi< T >::value * 2;
        };
    };
    radians_revolutions_linear_relation units_linear_relation_lookup(
        radian_traits&&,
        revolution_traits&&
    );
    
    // Arcminutes <-> X ////////////////////////////////////////////////////////
    
    struct arcminutes_arcseconds_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_den = (
                constants::minute_seconds< T >::value
            );
        };
    };
    arcminutes_arcseconds_linear_relation units_linear_relation_lookup(
        arcminute_traits&&,
        arcsecond_traits&&
    );
    
    struct arcminutes_revolutions_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                  constants::circle_degrees< T >::value
                * constants::hour_minutes< T >::value
            );
        };
    };
    arcminutes_revolutions_linear_relation units_linear_relation_lookup(
        arcminute_traits&&,
        revolution_traits&&
    );
    
    // Arcseconds <-> X ////////////////////////////////////////////////////////
    
    struct arcseconds_revolutions_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                  constants::circle_degrees< T >::value
                * constants::hour_minutes< T >::value
                * constants::minute_seconds< T >::value
            );
        };
    };
    arcseconds_revolutions_linear_relation units_linear_relation_lookup(
        arcsecond_traits&&,
        revolution_traits&&
    );
} }


#endif
