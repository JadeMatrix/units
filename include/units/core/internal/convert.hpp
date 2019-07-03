#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_CONVERT_HPP
#define JM_UNITS_CORE_INTERNAL_CONVERT_HPP


#include "core_type_detection.hpp"
#include "core_types.hpp"
#include "linear_relation.hpp"
#include "reduce.hpp"
#include "utils.hpp"    // remove_cvref_t

#include <functional>   // forward
#include <utility>      // declval
#include <ratio>
#include <type_traits>  // enable_if, is_same


namespace JadeMatrix { namespace units { namespace internal // Conversion switch
{
    template<
        template< typename > class To,
        template< typename > class From,
        typename = void
    > struct convertible
    {
        constexpr static auto is_fully = false;
    };
    
    template<
        template< typename > class To,
        template< typename > class From
    > struct convertible<
        To,
        From,
        typename std::enable_if< std::is_same<
            typename To  < void >::traits_type,
            typename From< void >::traits_type
        >::value >::type
    >
    {
        constexpr static auto is_fully = true;
        
        template< typename T > static constexpr T&& apply( T&& v )
        {
            return std::forward< T >( v );
        }
    };
    
    template<
        template< typename > class To,
        template< typename > class From
    > struct convertible<
        To,
        From,
        typename std::enable_if<
            !std::is_same<
                typename To  < void >::traits_type,
                typename From< void >::traits_type
            >::value
            && linear_relation_exists<
                typename To  < void >::traits_type,
                typename From< void >::traits_type
            >::value
        >::type
    >
    {
        constexpr static auto is_fully = true;
        
        using _relation = linear_relation<
            typename To  < void >::traits_type,
            typename From< void >::traits_type
        >;
        
        template< typename T > static constexpr auto apply( T&& v ) ->
            decltype( _relation::apply( std::forward< T >( v ) ) )
        {
            return _relation::apply( std::forward< T >( v ) );
        }
    };
    
    template<> struct convertible< ratio, ratio >
    {
        constexpr static auto is_fully = true;
        
        template< typename T > static constexpr T&& apply( T&& v )
        {
            return std::forward< T >( v );
        }
    };
} } }


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


namespace JadeMatrix { namespace units { namespace internal // Full conversion /
{
    template<
        template< typename > class To,
        template< typename > class From/*,
        typename = void*/
    > struct conversion
    {
        using   to_reduced = reduced<   To< void > >;
        using from_reduced = reduced< From< void > >;
        
        using _convertible = convertible<
              to_reduced::template unit_type,
            from_reduced::template unit_type
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
        };
        
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
        };
    };
} } }


#endif