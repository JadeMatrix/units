#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_LINEAR_RELATION_HPP
#define JM_UNITS_CORE_INTERNAL_LINEAR_RELATION_HPP


#include "core_type_detection.hpp"
#include "core_types.hpp"
#include "utils.hpp"    // void_t, remove_cvref_t

#include <utility>      // declval
#include <type_traits>  // true_type, false_type


namespace JadeMatrix { namespace units { namespace internal // One-way /////////
{
    template<
        typename YTraits,
        typename XTraits,
        typename = void
    > struct one_way_linear_relation;
    
    template<
        typename YTraits,
        typename XTraits
    > struct one_way_linear_relation<
        YTraits,
        XTraits,
        // ADL lookup must be duplicated here to take part in SFINAE
        void_t< decltype( units_linear_relation_lookup(
            std::declval< YTraits >(),
            std::declval< XTraits >()
        ) ) >
    >
    {
        using exists_type = void;
        
        // ADL lookup for linear relation type
        using _lookup = decltype( units_linear_relation_lookup(
            std::declval< YTraits >(),
            std::declval< XTraits >()
        ) );
        
        // In most cases this will just be T
        template< typename T > using _common_type = typename std::common_type<
            remove_cvref_t< T >,
            decltype( _lookup::template values< T >::slope_num ),
            decltype( _lookup::template values< T >::slope_den ),
            // TODO: Implement intercept of 0 if not in `_lookup`
            decltype( _lookup::template values< T >::intercept )
        >::type;
        
        template< typename T > static constexpr _common_type< T > apply( T&& v )
        {
            using ct = _common_type< T >;
            using values = typename _lookup::template values< T >;
            return (
                  static_cast< ct >( v )
                * static_cast< ct >( values::slope_num )
                / static_cast< ct >( values::slope_den )
                + static_cast< ct >( values::intercept )
            );
        }
    };
    
    template<
        typename YTraits,
        typename XTraits,
        typename = void
    > struct one_way_linear_relation_exists : std::false_type {};
    template<
        typename YTraits,
        typename XTraits
    > struct one_way_linear_relation_exists<
        YTraits,
        XTraits,
        typename one_way_linear_relation< YTraits, XTraits >::exists_type
    > : std::true_type {};
} } }


namespace JadeMatrix { namespace units { namespace internal // Two-way /////////
{
    template<
        typename YTraits,
        typename XTraits,
        typename = void
    > struct linear_relation;
    
    template<
        typename YTraits,
        typename XTraits
    > struct linear_relation<
        YTraits,
        XTraits,
        typename std::enable_if< one_way_linear_relation_exists<
            YTraits,
            XTraits
        >::value >::type
    > : one_way_linear_relation< YTraits, XTraits > {};
    
    template<
        typename YTraits,
        typename XTraits
    > struct linear_relation<
        YTraits,
        XTraits,
        typename std::enable_if<
            // If there is an explicit primary relation, prefer that
            !one_way_linear_relation_exists<
                YTraits,
                XTraits
            >::value
            && one_way_linear_relation_exists<
                XTraits,
                YTraits
            >::value
        >::type
    >
    {
        using exists_type = void;
        
        using _inverse = one_way_linear_relation< XTraits, YTraits >;
        using _lookup  = typename _inverse::_lookup;
        
        template<
            typename T
        > using _common_type = typename _inverse::template _common_type< T >;
        
        template< typename T > static constexpr _common_type< T > apply( T&& v )
        {
            using ct = _common_type< T >;
            using values = typename _lookup::template values< T >;
            return (
                (
                      static_cast< ct >( v )
                    - static_cast< ct >( values::intercept )
                )
                * static_cast< ct >( values::slope_den )
                / static_cast< ct >( values::slope_num )
            );
        }
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
        typename linear_relation< YTraits, XTraits >::exists_type
    > : std::true_type {};
} } }


#endif
