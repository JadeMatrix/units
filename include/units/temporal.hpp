#pragma once
#ifndef JM_UNITS_TEMPORAL_HPP
#define JM_UNITS_TEMPORAL_HPP


#include "core/constants.hpp"
#include "core/define_unit.hpp"

#include <chrono>


namespace JadeMatrix { namespace units // Declarations /////////////////////////
{
    struct                  second_traits {};
    struct                  minute_traits {};
    struct                    hour_traits {};
    // struct            standard_day_traits {};  //  24 hours
    // struct           standard_week_traits {};  //   7 days
    // struct          standard_month_traits {};  //  30 days
    // struct standard_gregorian_year_traits {};  // 365 days
    // struct           average_month_traits {};  //  30.436875 std. days
    // struct  average_gregorian_year_traits {};  // 365.2425   std. days
} }


namespace JadeMatrix { namespace units
{
    struct seconds_minutes_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                constants::minute_seconds< T >::value
            );
        };
    };
    seconds_minutes_linear_relation units_linear_relation_lookup(
        second_traits&&,
        minute_traits&&
    );
    
    struct seconds_hours_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                  constants::minute_seconds< T >::value
                * constants::minute_seconds< T >::value
            );
        };
    };
    seconds_hours_linear_relation units_linear_relation_lookup(
        second_traits&&,
        hour_traits&&
    );
    
    struct minutes_hours_linear_relation
    {
        template< typename T > struct values
        {
            static constexpr auto slope_num = (
                constants::minute_seconds< T >::value
            );
        };
    };
    minutes_hours_linear_relation units_linear_relation_lookup(
        minute_traits&&,
        hour_traits&&
    );
} }


namespace JadeMatrix { namespace units
{
    // Create a subclass of `unit` to add implicit conversions to & from
    // `std::chrono::duration<>` types
    
    #define JM_UNITS_INTERNAL_DEFINE_MOST_STRINGS_FOR_TEMPORAL_UNIT( \
        UNIT_PLURAL, \
        TRAITS, \
        RATIO, \
        UNIT_NAME_STR, \
        UNIT_SYM_STR \
    ) \
     \
    template< \
        typename Scale, \
        typename T = void \
    > struct _##UNIT_PLURAL##_unit : public unit< \
        TRAITS, \
        Scale, \
        T \
    > \
    { \
    public: \
        using unit< TRAITS, Scale, T >::unit; \
         \
        template< typename O > _##UNIT_PLURAL##_unit( \
            const std::chrono::duration< O, RATIO >& s \
        ) : \
            unit< TRAITS, Scale, T >( s.count() ) \
        {} \
         \
        template< typename O, typename R > _##UNIT_PLURAL##_unit( \
            const std::chrono::duration< O, R >& o \
        ) : \
            unit< TRAITS, Scale, T >( std::chrono::duration_cast< \
                std::chrono::duration< O > \
            >( o ).count() ) \
        {} \
         \
        template< \
            typename Rep, \
            typename Period \
        > constexpr operator std::chrono::duration< Rep, Period > () const \
        { \
            return std::chrono::duration< Rep, Period >{ this -> _value }; \
        } \
    }; \
     \
    template< typename Scale > struct _##UNIT_PLURAL##_unit< \
        Scale, \
        void \
    > : public unit< \
        TRAITS, \
        Scale, \
        void \
    > \
    {}; \
      \
    template< typename T > using         UNIT_PLURAL = _##UNIT_PLURAL##_unit<  unit_scale, T >; \
    template< typename T > using dozen_##UNIT_PLURAL = _##UNIT_PLURAL##_unit< dozen_scale, T >; \
    template< typename T > using     bi##UNIT_PLURAL = _##UNIT_PLURAL##_unit<    bi_scale, T >; \
    template< typename T > using   semi##UNIT_PLURAL = _##UNIT_PLURAL##_unit<  semi_scale, T >; \
     \
    template< typename T > using    exa##UNIT_PLURAL = _##UNIT_PLURAL##_unit<   exa_scale, T >; \
    template< typename T > using   peta##UNIT_PLURAL = _##UNIT_PLURAL##_unit<  peta_scale, T >; \
    template< typename T > using   tera##UNIT_PLURAL = _##UNIT_PLURAL##_unit<  tera_scale, T >; \
    template< typename T > using   giga##UNIT_PLURAL = _##UNIT_PLURAL##_unit<  giga_scale, T >; \
    template< typename T > using   mega##UNIT_PLURAL = _##UNIT_PLURAL##_unit<  mega_scale, T >; \
    template< typename T > using   kilo##UNIT_PLURAL = _##UNIT_PLURAL##_unit<  kilo_scale, T >; \
    template< typename T > using  hecto##UNIT_PLURAL = _##UNIT_PLURAL##_unit< hecto_scale, T >; \
    template< typename T > using   deca##UNIT_PLURAL = _##UNIT_PLURAL##_unit<  deca_scale, T >; \
     \
    template< typename T > using   deci##UNIT_PLURAL = _##UNIT_PLURAL##_unit<  deci_scale, T >; \
    template< typename T > using  centi##UNIT_PLURAL = _##UNIT_PLURAL##_unit< centi_scale, T >; \
    template< typename T > using  milli##UNIT_PLURAL = _##UNIT_PLURAL##_unit< milli_scale, T >; \
    template< typename T > using  micro##UNIT_PLURAL = _##UNIT_PLURAL##_unit< micro_scale, T >; \
    template< typename T > using   nano##UNIT_PLURAL = _##UNIT_PLURAL##_unit<  nano_scale, T >; \
    template< typename T > using   pico##UNIT_PLURAL = _##UNIT_PLURAL##_unit<  pico_scale, T >; \
    template< typename T > using  femto##UNIT_PLURAL = _##UNIT_PLURAL##_unit< femto_scale, T >; \
    template< typename T > using   atto##UNIT_PLURAL = _##UNIT_PLURAL##_unit<  atto_scale, T >; \
     \
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS( TRAITS, UNIT_NAME_STR, UNIT_SYM_STR )
    
    #if defined JM_UNITS_YT_AVAILABLE
        #define JM_UNITS_INTERNAL_DEFINE_STRINGS_FOR_TEMPORAL_UNIT( \
                UNIT_PLURAL, \
                TRAITS, \
                RATIO, \
                UNIT_NAME_STR, \
                UNIT_SYM_STR \
            ) \
        JM_UNITS_INTERNAL_DEFINE_MOST_STRINGS_FOR_TEMPORAL_UNIT( \
            UNIT_PLURAL, \
            TRAITS, \
            RATIO, \
            UNIT_NAME_STR, \
            UNIT_SYM_STR \
        ) \
        template< typename T > using zepto##UNIT_PLURAL = _##UNIT_PLURAL##_unit< zepto_scale, T >; \
        template< typename T > using zetta##UNIT_PLURAL = _##UNIT_PLURAL##_unit< zetta_scale, T >; \
        template< typename T > using yocto##UNIT_PLURAL = _##UNIT_PLURAL##_unit< yocto_scale, T >; \
        template< typename T > using yotta##UNIT_PLURAL = _##UNIT_PLURAL##_unit< yotta_scale, T >;
    #elif defined JM_UNITS_ZT_AVAILABLE
        #define JM_UNITS_INTERNAL_DEFINE_STRINGS_FOR_TEMPORAL_UNIT( \
                UNIT_PLURAL, \
                TRAITS, \
                RATIO, \
                UNIT_NAME_STR, \
                UNIT_SYM_STR \
            ) \
        JM_UNITS_INTERNAL_DEFINE_MOST_STRINGS_FOR_TEMPORAL_UNIT( \
            UNIT_PLURAL, \
            TRAITS, \
            RATIO, \
            UNIT_NAME_STR, \
            UNIT_SYM_STR \
        ) \
        template< typename T > using zepto##UNIT_PLURAL = _##UNIT_PLURAL##_unit< zepto_scale, T >; \
        template< typename T > using zetta##UNIT_PLURAL = _##UNIT_PLURAL##_unit< zetta_scale, T >;
    #else
        #define JM_UNITS_INTERNAL_DEFINE_STRINGS_FOR_TEMPORAL_UNIT( \
                UNIT_PLURAL, \
                TRAITS, \
                RATIO, \
                UNIT_NAME_STR, \
                UNIT_SYM_STR \
            ) \
        JM_UNITS_INTERNAL_DEFINE_MOST_STRINGS_FOR_TEMPORAL_UNIT( \
            UNIT_PLURAL, \
            TRAITS, \
            RATIO, \
            UNIT_NAME_STR, \
            UNIT_SYM_STR \
        )
    #endif
    
    JM_UNITS_INTERNAL_DEFINE_STRINGS_FOR_TEMPORAL_UNIT( seconds, second_traits,         unit_scale, "seconds",   "s" )
    JM_UNITS_INTERNAL_DEFINE_STRINGS_FOR_TEMPORAL_UNIT( minutes, minute_traits, std::ratio<   60 >, "minutes", "min" )
    JM_UNITS_INTERNAL_DEFINE_STRINGS_FOR_TEMPORAL_UNIT(   hours,   hour_traits, std::ratio< 3600 >,   "hours",   "h" )
} }


#endif
