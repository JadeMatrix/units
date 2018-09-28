#pragma once
#ifndef JM_UNITS_LINEAR_HPP
#define JM_UNITS_LINEAR_HPP


#include "unit.hpp"
#include "scales.hpp"
#include "strings.hpp"
#include "constants.hpp"


namespace JadeMatrix { namespace units
{
    struct  foot_traits;
    struct meter_traits;
    
    DEFINE_ALL_PREFIXES_FOR_UNIT(   feet,  foot_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT( meters, meter_traits )
    
    DEFINE_ALL_STRINGS_FOR_UNIT( feet  , "feet"  , "ft" )
    DEFINE_ALL_STRINGS_FOR_UNIT( meters, "meters", "m"  )
} }


namespace JadeMatrix { namespace units
{
    struct foot_traits
    {
        template< typename T, typename In > static T from( const In& );
    };
    struct meter_traits
    {
        template< typename T, typename In > static T from( const In& );
    };
    
    template< typename T > struct traits_linear_relation<
        foot_traits,
        meter_traits,
        T
    >
    {
        static constexpr T slope = constants< T >::meter_cms / (
              constants< T >::inch_cms
            * constants< T >::foot_inches
        );
        static constexpr T intercept = 0;
    };
} }


#endif
