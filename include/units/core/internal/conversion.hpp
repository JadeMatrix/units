#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_CONVERSION_HPP
#define JM_UNITS_CORE_INTERNAL_CONVERSION_HPP


#include "convertible.hpp"
#include "core_type_detection.hpp"
#include "reduce.hpp"
#include "scale.hpp"
#include "utils.hpp"    // remove_cvref_t

#include <utility>      // declval
#include <ratio>
#include <type_traits>  // enable_if


namespace JadeMatrix { namespace units { namespace internal // Full conversion /
{
    template<
        template< typename > class To,
        template< typename > class From/*,
        typename = void*/
    > struct conversion
    {
        using _convertible = convertible<
            reduced<   To< void > >::template unit_type,
            reduced< From< void > >::template unit_type
        >;
        static constexpr auto exists = _convertible::is_fully;
        
        template< typename T > using _result = remove_cvref_t< decltype(
            _convertible::apply(
                static_cast< T >(
                    std::declval< T >()
                )
            )
        ) >;
        
        // TODO: use this to support move semantics
        template< typename T > static constexpr auto apply( const From< T >& f )
            -> typename std::enable_if<
                exists && std::ratio_equal<
                    scale< To, From >,
                    std::ratio< 1 >
                >::value,
                _result< T >
            >::type
        {
            return _convertible::apply( static_cast< T >( f ) );
        }
        
        template< typename T > static constexpr auto apply( const From< T >& f )
            -> typename std::enable_if<
                exists && !std::ratio_equal<
                    scale< To, From >,
                    std::ratio< 1 >
                >::value,
                _result< T >
            >::type
        {
            return _convertible::apply(
                // Assuming the scale type is `std::ratio`, the types of the
                // scale's `num`/`den` will always be `std::intmax_t`, which may
                // otherwise poison the resulting type with widening.
                static_cast< T >(
                    static_cast< T >( f )
                    * scale< To, From >::den
                )
            ) / scale< To, From >::num;
        }
    };
} } }


#endif
