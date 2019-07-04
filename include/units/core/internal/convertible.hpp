#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_CONVERTIBLE_HPP
#define JM_UNITS_CORE_INTERNAL_CONVERTIBLE_HPP


#include "core_types.hpp"
#include "linear_relation.hpp"

#include <functional>   // forward
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
    // {
    //     constexpr static auto is_partially = ...;
    //     constexpr static auto is_fully     = ...;
    //     // The part of From convertible to To, or `ratio`
    //     template< typename T > using unit_type = ...;
    //     // The part of From not convertible to To, or `ratio`
    //     template< typename T > using rest_type = ...;
    // };
    
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


#endif
