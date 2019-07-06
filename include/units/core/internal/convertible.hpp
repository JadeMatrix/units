#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_CONVERTIBLE_HPP
#define JM_UNITS_CORE_INTERNAL_CONVERTIBLE_HPP


#include "core_types.hpp"
#include "linear_relation.hpp"
#include "scale.hpp"
#include "unit_list.hpp"    // sort, take_numer
#include "utils.hpp"        // remove_cvref_t

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
    
    // Reduction comparison to check for convertibility
    template<
        template< typename > class  FirstUnit,
        template< typename > class SecondUnit
    > struct compare_is_convertible
    {
        static constexpr auto value = convertible<
             FirstUnit,
            SecondUnit
        >::is_fully;
        
        using additional_scale = scale< FirstUnit, SecondUnit >;
    };
} } }


namespace JadeMatrix { namespace units { namespace internal // Convert `unit`s /
{
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
        
        template< typename T > static constexpr auto apply( T&& v )
            -> decltype( _relation::apply( std::forward< T >( v ) ) )
        {
            return _relation::apply( std::forward< T >( v ) );
        }
    };
} } }


namespace JadeMatrix { namespace units { namespace internal // Convert `by`s ///
{
    template<
        typename   ToList,
        typename FromList
    > struct convertible_by_impl_applier
    {
        using _first_convertible = convertible<
              ToList::template first_unit,
            FromList::template first_unit
        >;
        template< typename T > static constexpr auto apply( T&& v )
            -> decltype(
                _first_convertible::apply(
                    static_cast< remove_cvref_t< T > >( 1 )
                ) * convertible_by_impl_applier<
                    typename   ToList::rest_type,
                    typename FromList::rest_type
                >::apply( std::forward< T >( v ) )
            )
        {
            return (
                _first_convertible::apply(
                    static_cast< remove_cvref_t< T > >( 1 )
                ) * convertible_by_impl_applier<
                    typename   ToList::rest_type,
                    typename FromList::rest_type
                >::apply( std::forward< T >( v ) )
            );
        }
    };
    
    template<> struct convertible_by_impl_applier< unit_list<>, unit_list<> >
    {
        template< typename T > static constexpr T&& apply( T&& v )
        {
            return std::forward< T >( v );
        }
    };
    
    template<
        template< typename > class To,
        template< typename > class From,
        typename   ToList = typename take_numer<   To >::type,
        typename FromList = typename take_numer< From >::type,
        // Assume that the `by`s are already reduced halves of a `per` if fully-
        // reduced-conversion behavior is desired
        typename Sorted = sort< ToList, FromList, compare_is_convertible >,
        typename = typename std::enable_if< Sorted::possible >::type
    > struct convertible_by_impl
    {
        constexpr static auto is_fully = Sorted::possible;
        
        template< typename T > static constexpr auto apply( T&& v )
            -> decltype( convertible_by_impl_applier<
                typename Sorted:: first_list,
                typename Sorted::second_list
            >::apply( std::forward< T >( v ) ) )
        {
            return convertible_by_impl_applier<
                typename Sorted:: first_list,
                typename Sorted::second_list
            >::apply( std::forward< T >( v ) );
        }
    };
    
    template<
        template< typename > class To,
        template< typename > class From
    > struct convertible<
        To,
        From,
        typename std::enable_if<
            // Make the assumption that there will be no `unit`s directly
            // convertible to & from `by`s
               is_by<   To< void > >::value
            && is_by< From< void > >::value
        >::type
    > : convertible_by_impl< To, From > {};
} } }


namespace JadeMatrix { namespace units { namespace internal // Convert `per`s //
{
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
        
        using _numer_convertible = convertible<
              To< void >::template numer_unit,
            From< void >::template numer_unit
        >;
        using _denom_convertible = convertible<
              To< void >::template denom_unit,
            From< void >::template denom_unit
        >;
        
        template< typename T > static constexpr auto apply( T&& v )
            -> decltype(
                  _numer_convertible::apply( std::forward< T >( v ) )
                / _denom_convertible::apply( static_cast<
                    remove_cvref_t< T >
                >( 1 ) )
            )
        {
            return (
                  _numer_convertible::apply( std::forward< T >( v ) )
                / _denom_convertible::apply( static_cast<
                    remove_cvref_t< T >
                >( 1 ) )
            );
        }
    };
} } }


namespace JadeMatrix { namespace units { namespace internal // Convert `ratio`s
{
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
