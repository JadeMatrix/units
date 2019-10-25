#pragma once
#ifndef JM_UNITS_CORE_CONSTANTS_HPP
#define JM_UNITS_CORE_CONSTANTS_HPP


#include <type_traits>  // enable_if, is_floating_point


namespace JadeMatrix { namespace units { namespace constants
{
    namespace internal
    {
        // Explicit floating point narrowing logic so as not to trigger double
        // promotion warnings
        
        template<
            typename To,
            typename From,
            typename = void
        > struct narrow { using type = From; };
        template< typename To > struct narrow<
            To,
            long double,
            typename std::enable_if< std::is_floating_point< To >::value >::type
        > { using type = To; };
    }
    
    #define DEFINE_CONSTANT( NAME, TYPE, VALUE ) \
        template< typename T > struct NAME \
        { \
            static constexpr auto value = static_cast< \
                typename internal::narrow< T, TYPE >::type \
            >( VALUE ); \
        };
    
    // For integer constants, use the smallest int type to hold the value to
    // prevent unnecessary widening.
    // TODO: least_int_type< max_val >
    DEFINE_CONSTANT( pi                  , long double   , 3.1415926535897932385L )
    DEFINE_CONSTANT( circle_degrees      , unsigned short, 360   )
    DEFINE_CONSTANT( minute_seconds      , unsigned char , 60    )
    DEFINE_CONSTANT( hour_minutes        , unsigned char , 60    )
    DEFINE_CONSTANT( inch_cms            , long double   , 2.54L )
    DEFINE_CONSTANT( foot_inches         , unsigned char , 12    )
    DEFINE_CONSTANT( meter_cms           , unsigned char , 100   )
    DEFINE_CONSTANT( yard_feet           , unsigned char , 3     )
    DEFINE_CONSTANT( mile_feet           , unsigned short, 5280  )
    DEFINE_CONSTANT( fathom_yards        , unsigned char , 2     )
    DEFINE_CONSTANT( nautical_mile_meters, unsigned short, 1852  )
    
    #undef DEFINE_CONSTANT
} } }


#endif
