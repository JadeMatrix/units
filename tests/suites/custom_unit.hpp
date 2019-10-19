#pragma once
#ifndef JM_UNITS_TEST_CUSTOM_UNIT_HPP
#define JM_UNITS_TEST_CUSTOM_UNIT_HPP


#include <units/core/define_unit.hpp>


namespace custom
{
    struct foo_traits {};
    
    #define DEFINE_PREFIX_FOR_foos( PREFIX, SCALE ) \
        template< typename T > using PREFIX##foos = ::JadeMatrix::units::unit< \
            foo_traits, \
            SCALE, \
            T \
        >;
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_foos )
    #undef DEFINE_PREFIX_FOR_foos
    
    JM_UNITS_DEFINE_STRINGS_FOR_TRAITS( foo_traits, "foos", "¿" )
}


#endif
