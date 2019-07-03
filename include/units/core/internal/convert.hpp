#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_CONVERT_HPP
#define JM_UNITS_CORE_INTERNAL_CONVERT_HPP


#include "core_type_detection.hpp"
#include "core_types.hpp"
#include "linear_relation.hpp"

#include <functional>   // forward
#include <utility>      // declval
#include <ratio>        // ratio_divide
#include <type_traits>  // enable_if, is_same


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
                    static_cast< O >( static_cast< O >( o ) * scale::den )
                ) / scale::num
            );
        }
    };
} } }


#endif
