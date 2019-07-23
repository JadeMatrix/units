#pragma once
#ifndef JM_UNITS_TEST_CUSTOM_UNIT_HPP
#define JM_UNITS_TEST_CUSTOM_UNIT_HPP


#include <units/core/define_unit.hpp>


namespace custom
{
    struct foo_traits {};
    
    DEFINE_ALL_STRINGS_FOR_UNIT( foo_traits, "foos", "¿" )
    DEFINE_ALL_PREFIXES_FOR_UNIT( foos, custom::foo_traits )
}


#endif
