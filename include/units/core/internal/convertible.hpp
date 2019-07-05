#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_CONVERTIBLE_HPP
#define JM_UNITS_CORE_INTERNAL_CONVERTIBLE_HPP


#include "core_types.hpp"
#include "linear_relation.hpp"
#include "utils.hpp"    // remove_cvref_t

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
    
    // Basic `unit`, same traits
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
    
    // Basic `unit`, convertible traits
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
    
    // `by`
    // TODO:
    
    // `per`
    template<
        template< typename > class To,
        template< typename > class From
    > struct convertible<
        To,
        From,
        typename std::enable_if<
            // Make the assumption that there will be no `unit`s directly
            // convertible to & from `per`s
               is_per<   To< void > >::value
            && is_per< From< void > >::value
            && convertible<
                  To< void >::template numer_unit,
                From< void >::template numer_unit
            >::is_fully
            && convertible<
                  To< void >::template denom_unit,
                From< void >::template denom_unit
            >::is_fully
        >::type
    >
    {
        constexpr static auto is_fully = true;
        
        template< typename T > static constexpr T&& apply( T&& v )
        {
            using numer_convertible = convertible<
                  To< void >::template numer_unit,
                From< void >::template numer_unit
            >;
            using denom_convertible = convertible<
                  To< void >::template denom_unit,
                From< void >::template denom_unit
            >;
            return (
                  numer_convertible::apply( std::forward< T >( v ) )
                / denom_convertible::apply( static_cast<
                    remove_cvref_t< T >
                >( 1 ) )
            );
        }
    };
    
    // `ratio`
    template<
        template< typename > class To,
        template< typename > class From
    > struct convertible<
        To,
        From,
        // Can't use basic template specialization on `ratio` as both GCC and
        // Clang fail to match aliased `ratio`s in some cases (known issue
        // caused by ambiguous wording in the standard)
        typename std::enable_if<
               is_ratio<   To< void > >::value
            && is_ratio< From< void > >::value
        >::type
    >
    {
        constexpr static auto is_fully = true;
        
        template< typename T > static constexpr T&& apply( T&& v )
        {
            return std::forward< T >( v );
        }
    };
} } }


#endif
