#pragma once
#ifndef JM_UNITS_CORE_CONSTANTS_HPP
#define JM_UNITS_CORE_CONSTANTS_HPP


namespace JadeMatrix { namespace units { namespace constants
{
    #define DEFINE_CONSTANT( NAME, TYPE, VALUE ) \
    template< typename T > struct NAME { static constexpr TYPE value = VALUE; };
    
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
    
    #define DEFINE_EXPLICIT_CAST_CONSTANT( NAME, TYPE ) \
    template<> struct NAME< TYPE >{ \
        static constexpr TYPE value = static_cast< TYPE >( \
            NAME< void >::value \
        ); \
    };
    
    // Explicit floating point casts so as not to trigger double promotion
    // warnings
    DEFINE_EXPLICIT_CAST_CONSTANT( pi      , double )
    DEFINE_EXPLICIT_CAST_CONSTANT( pi      , float  )
    DEFINE_EXPLICIT_CAST_CONSTANT( inch_cms, double )
    DEFINE_EXPLICIT_CAST_CONSTANT( inch_cms, float  )
    
    #undef DEFINE_EXPLICIT_CAST_CONSTANT
} } }


#endif
