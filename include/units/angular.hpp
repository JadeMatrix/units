#pragma once
#ifndef JM_UNITS_ANGULAR_HPP
#define JM_UNITS_ANGULAR_HPP


#include "units.hpp"
#include "scales.hpp"
#include "strings.hpp"
#include "internal/constants.hpp"


namespace JadeMatrix { namespace units // Declarations /////////////////////////
{
    struct    degree_traits;
    struct    radian_traits;
    struct arcminute_traits;
    
    DEFINE_ALL_PREFIXES_FOR_UNIT(    degrees,    degree_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT(    radians,    radian_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT( arcminutes, arcminute_traits )
    
    DEFINE_ALL_STRINGS_FOR_UNIT( degrees   , "degrees"   , "°"   )
    DEFINE_ALL_STRINGS_FOR_UNIT( radians   , "radians"   , "r"   )
    DEFINE_ALL_STRINGS_FOR_UNIT( arcminutes, "arcminutes", "moa" )
} }


namespace JadeMatrix { namespace units
{
    struct degree_traits
    {
        template< typename T, typename In > static T from( const In& );
    };
    struct radian_traits
    {
        template< typename T, typename In > static T from( const In& );
    };
    struct arcminute_traits
    {
        template< typename T, typename In > static T from( const In& );
    };
    
    template< typename T > struct traits_linear_relation<
        degree_traits,
        radian_traits,
        T
    >
    {
        static constexpr T slope = (
            constants< T >::circle_degrees
            / ( 2 * constants< T >::pi )
        );
        static constexpr T intercept = 0;
    };
    template< typename T > struct traits_linear_relation<
        degree_traits,
        arcminute_traits,
        T
    >
    {
        static constexpr T slope = 1 / constants< T >::hour_minutes;
        static constexpr T intercept = 0;
    };
    template< typename T > struct traits_linear_relation<
        arcminute_traits,
        radian_traits,
        T
    >
    {
        static constexpr T slope = traits_linear_relation<
            degree_traits,
            radian_traits,
            T
        >::slope / traits_linear_relation<
            degree_traits,
            arcminute_traits,
            T
        >::slope;
        static constexpr T intercept = 0;
    };
} }


#endif
