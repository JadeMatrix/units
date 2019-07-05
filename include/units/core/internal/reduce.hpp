#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_REDUCE_HPP
#define JM_UNITS_CORE_INTERNAL_REDUCE_HPP


#include "convertible.hpp"
#include "core_type_detection.hpp"
#include "unit_list.hpp"

#include <type_traits>  // is_same


/*

Okay, here's the plan...

1.  `unit`, `ratio` get reduced to that same type.
2.  `by` and `per` get expanded to two lists, numerator and denominator units,
    then...
3.  These two lists then have all pairs of the same units removed (e.g. if feet
    appears both in the numer and denom, one instance of each is removed from
    each list).
4.  These two lists are then recombined into a new unit based on their lengths:
    numer/denom 0/0 is a `ratio`, n/d 1/0 is numer, n/d N/0 is a `by`, and n/d
    N/N is a `per`.
5.  Voila, reduced; enjoy your compile times.

*/


namespace JadeMatrix { namespace units { namespace internal // Reduction compare
{
    template<
        template< typename > class  FirstUnit,
        template< typename > class SecondUnit
    > struct compare_is_same : std::is_same<
        // WATCHME: may need a wrapper for this as `std::is_same` does not fully
        // resolve type through aliases with some compilers
         FirstUnit< void >,
        SecondUnit< void >
    >
    {};
    
    template<
        template< typename > class  FirstUnit,
        template< typename > class SecondUnit
    > struct compare_is_convertible
    {
        static constexpr auto value = convertible<
             FirstUnit,
            SecondUnit
        >::is_fully;
    };
} } }


namespace JadeMatrix { namespace units { namespace internal // Reduce //////////
{
    // Reduce a template unit type by converting to a `per`-of-`by`s and
    // cancelling units.
    
    template<
        template< typename > class Unit,
        template<
            template< typename > class,
            template< typename > class
        > class Compare,
        typename = void
    > struct basic_reduce;
    
    template<
        template< typename > class Unit,
        template<
            template< typename > class,
            template< typename > class
        > class Compare
    > struct basic_reduce<
        Unit,
        Compare,
        typename std::enable_if<
            is_basic_unit< Unit< void > >::value
            ||   is_ratio< Unit< void > >::value
        >::type
    >
    {
        template< typename T > using unit_type = Unit< T >;
    };
    
    template<
        template< typename > class Unit,
        template<
            template< typename > class,
            template< typename > class
        > class Compare
    > struct basic_reduce<
        Unit,
        Compare,
        typename std::enable_if<
              is_per< Unit< void > >::value
            || is_by< Unit< void > >::value
        >::type
    >
    {
        using _simplified = simplify<
            typename take_numer< Unit >::type,
            typename take_denom< Unit >::type,
            Compare
        >;
        
        template< typename T > using unit_type = typename unit_lists_to_unit<
            typename _simplified:: first_list,
            typename _simplified::second_list
        >::template unit_type< T >;
    };
    
    template<
        template< typename > class Unit
    > using reduce = basic_reduce< Unit, compare_is_same >;
    template<
        template< typename > class Unit
    > using full_reduce = basic_reduce< Unit, compare_is_convertible >;
    
    // Shortcut aliased to the reduced version of T
    template< typename T > using reduced = typename reduce<
        T::template unit_type
    >::template unit_type< typename T::value_type >;
    template< typename T > using full_reduced = typename full_reduce<
        T::template unit_type
    >::template unit_type< typename T::value_type >;
} } }


#endif
