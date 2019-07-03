#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_LINEAR_RELATION_HPP
#define JM_UNITS_CORE_INTERNAL_LINEAR_RELATION_HPP


#include "core_type_detection.hpp"
#include "core_types.hpp"
#include "utils.hpp"    // void_t

#include <utility>      // declval
#include <type_traits>  // true_type, false_type


namespace JadeMatrix { namespace units { namespace internal // Linear relation /
{
    template<
        typename YTraits,
        typename XTraits,
        typename = void
    > struct linear_relation
    {
        using _lookup = decltype( units_linear_relation_lookup(
            std::declval< YTraits >(),
            std::declval< XTraits >()
        ) );
        
        template< typename T > struct values
        {
            static constexpr auto slope_num = _lookup::template values< T >::slope_num;
            static constexpr auto slope_den = _lookup::template values< T >::slope_den;
            // TODO: Implement intercept of 0 if not in `_lookup`
            static constexpr auto intercept = _lookup::template values< T >::intercept;
        };
    };
    
    template<
        typename YTraits,
        typename XTraits,
        typename = void
    > struct linear_relation_exists : std::false_type {};
    template<
        typename YTraits,
        typename XTraits
    > struct linear_relation_exists<
        YTraits,
        XTraits,
        // ADL lookup for linear relation type
        void_t< decltype( units_linear_relation_lookup(
            std::declval< YTraits >(),
            std::declval< XTraits >()
        ) ) >
    > : std::true_type {};
} } }


#endif
