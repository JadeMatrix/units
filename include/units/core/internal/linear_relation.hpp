#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_LINEAR_RELATION_HPP
#define JM_UNITS_CORE_INTERNAL_LINEAR_RELATION_HPP


#include "core_type_detection.hpp"
#include "core_types.hpp"
#include "utils.hpp"    // internal::void_t

#include <functional>   // forward
#include <utility>      // declval
#include <ratio>        // ratio_divide
#include <string>
#include <type_traits>  // true_type, false_type, enable_if, is_same


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


namespace JadeMatrix { namespace units { namespace internal // Basic conversion
{
    template<
        typename Traits,
        typename OtherTraits,
        typename = void
    > struct basic_conversion;
    
    template<
        typename Traits,
        typename OtherTraits
    > struct basic_conversion<
        Traits,
        OtherTraits,
        typename std::enable_if< std::is_same<
            Traits,
            OtherTraits
        >::value >::type
    >
    {
        template< typename T > static constexpr T value( T&& v )
        {
            return std::forward< T >( v );
        }
    };
    
    template<
        typename Traits,
        typename OtherTraits
    > struct basic_conversion<
        Traits,
        OtherTraits,
        typename std::enable_if< 
            !std::is_same< Traits, OtherTraits >::value
            && linear_relation_exists< Traits, OtherTraits >::value
        >::type
    >
    {
        template< typename T > using relvals = typename linear_relation<
            Traits,
            OtherTraits
        >::template values< T >;
        template< typename T > using common_type = typename std::common_type<
            decltype( relvals< T >::slope_num ),
            decltype( relvals< T >::slope_den ),
            decltype( relvals< T >::intercept ),
            T
        >::type;
        
        template< typename T > static constexpr common_type< T > value( T&& v )
        {
            using ct = common_type< T >;
            return (
                  static_cast< ct >( v                       )
                * static_cast< ct >( relvals< T >::slope_num )
                / static_cast< ct >( relvals< T >::slope_den )
                + static_cast< ct >( relvals< T >::intercept )
            );
        }
    };
    
    template<
        typename Traits,
        typename OtherTraits
    > struct basic_conversion<
        Traits,
        OtherTraits,
        typename std::enable_if< 
            !std::is_same< Traits, OtherTraits >::value
            && linear_relation_exists< OtherTraits, Traits >::value
        >::type
    >
    {
        template< typename T > using relvals = typename linear_relation<
            OtherTraits,
            Traits
        >::template values< T >;
        template< typename T > using common_type = typename std::common_type<
            decltype( relvals< T >::slope_num ),
            decltype( relvals< T >::slope_den ),
            decltype( relvals< T >::intercept ),
            T
        >::type;
        
        template< typename T > static constexpr common_type< T > value( T&& v )
        {
            using ct = common_type< T >;
            return (
                (
                      static_cast< ct >( v                       )
                    - static_cast< ct >( relvals< T >::intercept )
                )
                * static_cast< ct >( relvals< T >::slope_den )
                / static_cast< ct >( relvals< T >::slope_num )
            );
        }
    };
} } }


namespace JadeMatrix { namespace units { namespace internal // Full conversion /
{
    template< template< typename > class Unit > struct conversion
    {
        template<
            template< typename > class OtherUnit
        > using _convert_from = basic_conversion<
            typename      Unit< void >::traits_type,
            typename OtherUnit< void >::traits_type
        >;
        template<
            template< typename > class OtherUnit
        > using _scale = std::ratio_divide<
            typename      Unit< void >::scale_type,
            typename OtherUnit< void >::scale_type
        >;
        
        // Only supports basic units for now
        template<
            typename OtherTraits,
            typename OtherScale
        > struct _other_basic_unit
        {
            template< typename T > using type = typename unit<
                OtherTraits,
                OtherScale,
                void
            >::template unit_type< T >;
            
            template< typename T > using resulting_type = decltype(
                _convert_from< type >::value( std::declval< T >() )
            );
        };
        template<
            typename OtherTraits,
            typename OtherScale,
            typename O
        > static constexpr auto from(
            const unit< OtherTraits, OtherScale, O >& o
        ) -> typename _other_basic_unit<
            OtherTraits,
            OtherScale
        >::template resulting_type< O >
        {
            using scale = _scale< _other_basic_unit<
                OtherTraits,
                OtherScale
            >::template type >;
            
            return static_cast< typename _other_basic_unit<
                OtherTraits,
                OtherScale
            >::template resulting_type< O > >(
                _convert_from< _other_basic_unit<
                    OtherTraits,
                    OtherScale
                >::template type >::value(
                    // Assuming the scale type is `std::ratio`, the types of the
                    // scale's `num`/`den` will always be `std::intmax_t`, which
                    // may otherwise poison the resulting type with widening.
                    static_cast< O >(
                        static_cast< O >( o )
                        * scale::den
                        / scale::num
                    )
                )
            );
        }
    };
    
    template<
        template< typename > class ToUnit,
        template< typename > class FromUnit,
        typename = void
    > struct conversion_exists : std::false_type {};
    template<
        template< typename > class ToUnit,
        template< typename > class FromUnit
    > struct conversion_exists<
        ToUnit,
        FromUnit,
        void_t< decltype(
            conversion< ToUnit >::from( std::declval< FromUnit< void > >() )
        ) >
    > : std::true_type {};
} } }


#endif
