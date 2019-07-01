#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_LINEAR_RELATION_HPP
#define JM_UNITS_CORE_INTERNAL_LINEAR_RELATION_HPP


#include "core_types.hpp"
#include "utils.hpp"    // internal::void_t

#include <string>
#include <type_traits>  // true_type, false_type, enable_if, is_same


namespace JadeMatrix { namespace units { namespace internal // Unit conversions
{
    // TODO: Replace with ADL-on-traits trick
    template<
        typename Y_Traits,
        typename X_Traits,
        typename T
    > struct traits_linear_relation;
    
    
    template<
        typename Y_Traits,
        typename X_Traits,
        typename T,
        typename = void
    > struct linear_relation_exists : std::false_type {};
    template<
        typename Y_Traits,
        typename X_Traits,
        typename T
    > struct linear_relation_exists<
        Y_Traits,
        X_Traits,
        T,
        void_t<
            decltype( traits_linear_relation<
                Y_Traits,
                X_Traits,
                T
            >::slope_num ),
            decltype( traits_linear_relation<
                Y_Traits,
                X_Traits,
                T
            >::slope_den ),
            decltype( traits_linear_relation<
                Y_Traits,
                X_Traits,
                T
            >::intercept )
        >
    > : std::true_type {};
    
    
    template< typename Traits, typename T > struct traits_convert
    {
        template<
            typename Other_Traits,
            typename O
        > static constexpr auto from( const O& o )
            -> typename std::enable_if<
                std::is_same< Traits, Other_Traits >::value,
                T
            >::type
        {
            return o;
        }
        
        template<
            typename Other_Traits,
            typename O
        > static constexpr auto from( const O& o )
            -> typename std::enable_if<
                !std::is_same< Traits, Other_Traits >::value
                && linear_relation_exists< Traits, Other_Traits, O >::value,
                T
            >::type
        {
            using relation = traits_linear_relation< Traits, Other_Traits, O >;
            return (
                static_cast< O >( o )
                * relation::slope_num
                / relation::slope_den
                + relation::intercept
            );
        }
        
        template<
            typename Other_Traits,
            typename O
        > static constexpr auto from( const O& o )
            -> typename std::enable_if<
                !std::is_same< Traits, Other_Traits >::value
                && linear_relation_exists< Other_Traits, Traits, O >::value,
                T
            >::type
        {
            using relation = traits_linear_relation< Other_Traits, Traits, O >;
            return ( (
                static_cast< O >( o )
                - relation::intercept
            ) * relation::slope_den / relation::slope_num );
        }
    };
    
    
    template<
        typename Traits,
        typename T,
        typename Other_Traits,
        typename O,
        typename = void
    > struct conversion_exists : std::false_type {};
    template<
        typename Traits,
        typename T,
        typename Other_Traits,
        typename O
    > struct conversion_exists<
        Traits,
        T,
        Other_Traits,
        O,
        void_t< decltype(
            traits_convert< Traits, T >::template from< Other_Traits, O >
        ) >
    > : std::true_type {};
} } }


#endif
