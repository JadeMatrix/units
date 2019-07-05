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
        template< typename > class From,
        typename = void
    > struct conversion;
    
    template<
        template< typename > class To,
        template< typename > class From
    > struct conversion<
        To,
        From,
        typename std::enable_if< convertible<
            full_reduce<   To >::template unit_type,
            full_reduce< From >::template unit_type
        >::is_fully >::type
    >
    {
        using _convertible = convertible<
            full_reduce<   To >::template unit_type,
            full_reduce< From >::template unit_type
        >;
        static constexpr auto exists = _convertible::is_fully;
        
        using _additional_scale = std::ratio_divide<
            typename full_reduce< From >::additional_scale,
            typename full_reduce<   To >::additional_scale
        >;
        using final_scale = std::ratio_divide<
            scale< To, From >,
            _additional_scale
        >;
        
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
                std::ratio_equal< final_scale, std::ratio< 1 > >::value,
                _result< T >
            >::type
        {
            return _convertible::apply( static_cast< T >( f ) );
        }
        
        template< typename T > static constexpr auto apply( const From< T >& f )
            -> typename std::enable_if<
                !std::ratio_equal< final_scale, std::ratio< 1 > >::value,
                _result< T >
            >::type
        {
            return _convertible::apply(
                // Assuming the scale type is `std::ratio`, the types of the
                // scale's `num`/`den` will always be `std::intmax_t`, which may
                // otherwise poison the resulting type with widening.
                static_cast< T >(
                    static_cast< T >( f )
                    * final_scale::den
                )
            ) / final_scale::num;
        }
    };
} } }


#endif
