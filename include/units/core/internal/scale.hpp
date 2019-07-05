#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_SCALE_HPP
#define JM_UNITS_CORE_INTERNAL_SCALE_HPP


#include "core_type_detection.hpp"

#include <ratio>
#include <type_traits>  // enable_if


namespace JadeMatrix { namespace units { namespace internal // Scaling /////////
{
    template<
        template< typename > class To,
        template< typename > class From,
        typename = void
    > struct scale : std::ratio< 1 > {};
    
    template<
        template< typename > class To,
        template< typename > class From
    > struct scale<
        To,
        From,
        typename std::enable_if<
                is_basic_unit<   To< void > >::value
            && !is_basic_unit< From< void > >::value
        >::type
    > : To< void >::scale_type {};
    
    template<
        template< typename > class To,
        template< typename > class From
    > struct scale<
        To,
        From,
        typename std::enable_if<
              !is_basic_unit<   To< void > >::value
            && is_basic_unit< From< void > >::value
        >::type
    > : std::ratio_divide<
        std::ratio< 1 >,
        typename From< void >::scale_type
    > {};
    
    template<
        template< typename > class To,
        template< typename > class From
    > struct scale<
        To,
        From,
        typename std::enable_if<
               is_basic_unit<   To< void > >::value
            && is_basic_unit< From< void > >::value
        >::type
    > : std::ratio_divide<
        typename   To< void >::scale_type,
        typename From< void >::scale_type
    > {};
} } }


#endif
