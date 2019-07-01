#pragma once
#ifndef JM_UNITS_CORE_DEFINE_UNIT_HPP
#define JM_UNITS_CORE_DEFINE_UNIT_HPP


// These are all the necessary headers for defining custom units
#include "scales.hpp"
#include "strings.hpp"
#include "units.hpp"


// Define functions for unit strings with ADL on traits type; call this macro in
// the same namespace as the traits type is defined.  Uses `const&` of the
// traits type just in case it isn't trivially copyable.
#define DEFINE_ALL_STRINGS_FOR_UNIT( \
    TRAITS_TYPE, \
    UNIT_NAME_STR, \
    UNIT_SYM_STR \
) \
const std::string& unit_name( const TRAITS_TYPE& ) \
{ \
    static const std::string s{ UNIT_NAME_STR }; \
    return s; \
} \
const std::string& unit_symbol( const TRAITS_TYPE& ) \
{ \
    static const std::string s{ UNIT_SYM_STR }; \
    return s; \
}


#endif
