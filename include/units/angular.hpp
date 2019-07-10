#pragma once
#ifndef JM_UNITS_ANGULAR_HPP
#define JM_UNITS_ANGULAR_HPP


#include "core/constants.hpp"
#include "core/define_unit.hpp"


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
    
    struct degrees_radians_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                constants::circle_degrees< T >::value
                / /*static_cast< T >*/( 2 )
            );
            static constexpr auto slope_den = constants::pi< T >::value;
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
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
            static constexpr unsigned char slope_num = /*static_cast< T >*/( 1 );
            static constexpr auto slope_den = constants::hour_minutes< T >::value;
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
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
            static constexpr unsigned char slope_num = /*static_cast< T >*/( 1 );
            static constexpr auto slope_den = (
                  constants::hour_minutes< T >::value
                * constants::minute_seconds< T >::value
            );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
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
            static constexpr auto slope_num = constants::circle_degrees< T >::value;
            static constexpr unsigned char slope_den = /*static_cast< T >*/( 1 );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
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
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
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
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
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
            static constexpr unsigned char slope_den = /*static_cast< T >*/( 1 );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
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
            static constexpr unsigned char slope_num = /*static_cast< T >*/( 1 );
            static constexpr auto slope_den = constants::minute_seconds< T >::value;
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
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
            static constexpr unsigned char slope_den = /*static_cast< T >*/( 1 );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
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
            static constexpr unsigned char slope_den = /*static_cast< T >*/( 1 );
            static constexpr unsigned char intercept = /*static_cast< T >*/( 0 );
        };
    };
    arcseconds_revolutions_linear_relation units_linear_relation_lookup(
        arcsecond_traits&&,
        revolution_traits&&
    );
} }


#endif
