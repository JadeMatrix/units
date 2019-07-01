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
#define DEFINE_ALL_STRINGS_FOR_UNIT( \
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


#endif
