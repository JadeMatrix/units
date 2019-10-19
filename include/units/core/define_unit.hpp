#pragma once
#ifndef JM_UNITS_CORE_DEFINE_UNIT_HPP
#define JM_UNITS_CORE_DEFINE_UNIT_HPP


// These are all the necessary headers for defining custom units
#include "scales.hpp"
#include "strings.hpp"
#include "units.hpp"


// Define functions for unit strings with ADL on traits type; call this macro in
// the same namespace as the traits type is defined.  Uses a struct returned by
// an undefined lookup function to avoid instantiation of the traits type (via
// `std::declval<>()`).
#define JM_UNITS_DEFINE_STRINGS_FOR_TRAITS( \
    TRAITS_TYPE, \
    UNIT_NAME_STR, \
    UNIT_SYM_STR \
) \
struct units_##TRAITS_TYPE##_unit_strings \
{ \
    static const std::string& unit_name() \
    { \
        static const std::string s{ UNIT_NAME_STR }; \
        return s; \
    } \
    static const std::string& unit_symbol() \
    { \
        static const std::string s{ UNIT_SYM_STR }; \
        return s; \
    } \
}; \
units_##TRAITS_TYPE##_unit_strings units_unit_strings_lookup( TRAITS_TYPE&& );


// Helpers for defining prefixed units
#define JM_UNITS_INTERNAL_FOREACH_SCALE_UNIT( MACRO ) \
    MACRO( , ::JadeMatrix::units::unit_scale )

#define JM_UNITS_INTERNAL_FOREACH_SCALE_UNDERSCORE( MACRO ) \
    MACRO( dozen_, ::JadeMatrix::units::dozen_scale )

#define JM_UNITS_INTERNAL_FOREACH_SCALE_MOST( MACRO ) \
    MACRO(    bi, ::JadeMatrix::units::   bi_scale ) \
    MACRO(  semi, ::JadeMatrix::units:: semi_scale ) \
    MACRO(   exa, ::JadeMatrix::units::  exa_scale ) \
    MACRO(  peta, ::JadeMatrix::units:: peta_scale ) \
    MACRO(  tera, ::JadeMatrix::units:: tera_scale ) \
    MACRO(  giga, ::JadeMatrix::units:: giga_scale ) \
    MACRO(  mega, ::JadeMatrix::units:: mega_scale ) \
    MACRO(  kilo, ::JadeMatrix::units:: kilo_scale ) \
    MACRO( hecto, ::JadeMatrix::units::hecto_scale ) \
    MACRO(  deca, ::JadeMatrix::units:: deca_scale ) \
    MACRO(  deci, ::JadeMatrix::units:: deci_scale ) \
    MACRO( centi, ::JadeMatrix::units::centi_scale ) \
    MACRO( milli, ::JadeMatrix::units::milli_scale ) \
    MACRO( micro, ::JadeMatrix::units::micro_scale ) \
    MACRO(  nano, ::JadeMatrix::units:: nano_scale ) \
    MACRO(  pico, ::JadeMatrix::units:: pico_scale ) \
    MACRO( femto, ::JadeMatrix::units::femto_scale ) \
    MACRO(  atto, ::JadeMatrix::units:: atto_scale ) \
    MACRO(  exbi, ::JadeMatrix::units:: exbi_scale ) \
    MACRO(  pebi, ::JadeMatrix::units:: pebi_scale ) \
    MACRO(  tebi, ::JadeMatrix::units:: tebi_scale ) \
    MACRO(  gibi, ::JadeMatrix::units:: gibi_scale ) \
    MACRO(  mebi, ::JadeMatrix::units:: mebi_scale ) \
    MACRO(  kibi, ::JadeMatrix::units:: kibi_scale )

#if defined JM_UNITS_YT_AVAILABLE
    #define JM_UNITS_INTERNAL_FOREACH_SCALE_YT( MACRO ) \
        MACRO( yocto, ::JadeMatrix::units::yocto_scale ) \
        MACRO( yotta, ::JadeMatrix::units::yotta_scale ) \
        MACRO(  yobi, ::JadeMatrix::units:: yobi_scale )
#else
    #define JM_UNITS_INTERNAL_FOREACH_SCALE_YT( MACRO )
#endif

#if defined JM_UNITS_ZT_AVAILABLE
    #define JM_UNITS_INTERNAL_FOREACH_SCALE_ZT( MACRO ) \
        MACRO( zepto, ::JadeMatrix::units::zepto_scale ) \
        MACRO( zetta, ::JadeMatrix::units::zetta_scale ) \
        MACRO(  zebi, ::JadeMatrix::units:: zebi_scale )
#else
    #define JM_UNITS_INTERNAL_FOREACH_SCALE_ZT( MACRO )
#endif

#define JM_UNITS_FOREACH_SCALE( MACRO ) \
    JM_UNITS_INTERNAL_FOREACH_SCALE_UNIT      ( MACRO ) \
    JM_UNITS_INTERNAL_FOREACH_SCALE_UNDERSCORE( MACRO ) \
    JM_UNITS_INTERNAL_FOREACH_SCALE_MOST      ( MACRO ) \
    JM_UNITS_INTERNAL_FOREACH_SCALE_YT        ( MACRO ) \
    JM_UNITS_INTERNAL_FOREACH_SCALE_ZT        ( MACRO )


#endif
