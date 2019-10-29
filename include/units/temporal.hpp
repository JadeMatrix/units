#pragma once
#ifndef JM_UNITS_TEMPORAL_HPP
#define JM_UNITS_TEMPORAL_HPP


#include "core/constants.hpp"
#include "core/define_unit.hpp"

#include <chrono>
#include <type_traits>  // enable_if, true_type, false_type
#include <utility>      // declval


namespace JadeMatrix { namespace units
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
    // Specialization of `unit<>` that supports conversion to & from
    // `std::chrono::duration`s
    
    namespace internal
    {
        template<
            typename T,
            typename = void
        > struct is_std_chrono_duration : std::false_type {};
        template< typename T > struct is_std_chrono_duration<
            T,
            void_t< decltype(
                std::chrono::duration_cast< T >( std::declval< T >() )
            ) >
        > : std::true_type {};
    }
    
    #define DEFINE_UNIT_SPECIALIZATION_FOR( TRAITS, CHRONO_RATIO ) \
    template< \
        typename Scale, \
        typename T \
    > class unit< TRAITS, Scale, T > : \
        public internal::unit_impl< TRAITS, Scale, T > \
    { \
    protected: \
        using impl_type = internal::unit_impl< TRAITS, Scale, T >; \
         \
    public: \
        using internal::unit_impl< TRAITS, Scale, T >::unit_impl; \
         \
        template< typename Rep, typename Period > unit( \
            const std::chrono::duration< Rep, Period >& s \
        ) : impl_type( std::chrono::duration< \
            typename impl_type::value_type, \
            std::ratio_multiply< \
                CHRONO_RATIO, \
                typename impl_type::scale_type \
            > \
        >( s ).count() ) \
        {} \
         \
        template< \
            typename O, \
            typename = typename std::enable_if< \
                   !internal::is_unit               < O >::value \
                && !internal::is_std_chrono_duration< O >::value \
            >::type \
        > explicit constexpr operator O () const \
        { \
            return static_cast< O >( impl_type::_value ); \
        } \
         \
        template< \
            typename Rep, \
            typename Period \
        > operator std::chrono::duration< Rep, Period >() \
        { \
            return std::chrono::duration< \
                typename impl_type::value_type, \
                std::ratio_multiply< \
                    CHRONO_RATIO, \
                    typename impl_type::scale_type \
                > \
            >( impl_type::_value ); \
        } \
    };
    
    DEFINE_UNIT_SPECIALIZATION_FOR( second_traits, std::ratio<    1 > )
    DEFINE_UNIT_SPECIALIZATION_FOR( minute_traits, std::ratio<   60 > )
    DEFINE_UNIT_SPECIALIZATION_FOR(   hour_traits, std::ratio< 3600 > )
    
    #undef DEFINE_UNIT_SPECIALIZATION_FOR
} }


namespace JadeMatrix { namespace units
{
    #define DEFINE_PREFIX_FOR_seconds( PREFIX, SCALE ) template< typename T > using PREFIX##seconds = unit< second_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_minutes( PREFIX, SCALE ) template< typename T > using PREFIX##minutes = unit< minute_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_hours(   PREFIX, SCALE ) template< typename T > using PREFIX##hours   = unit<   hour_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_hertz(   PREFIX, SCALE ) template< typename T > using PREFIX##hertz   = per< ratio, PREFIX##seconds, T >;
    
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_seconds )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_minutes )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_hours   )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_hertz   )
    
    #undef DEFINE_PREFIX_FOR_seconds
    #undef DEFINE_PREFIX_FOR_minutes
    #undef DEFINE_PREFIX_FOR_hours
    #undef DEFINE_PREFIX_FOR_hertz
    
    
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS( second_traits, "seconds", "s" )
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS( minute_traits, "minutes", "m" )
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS(   hour_traits, "hours"  , "h" )
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


#endif
