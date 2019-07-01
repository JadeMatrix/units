#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_REDUCE_HPP
#define JM_UNITS_CORE_INTERNAL_REDUCE_HPP


#include "traits_utils.hpp"
#include "unit_list.hpp"


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


namespace JadeMatrix { namespace units { namespace internal // Reduce //////////
{
    // Reduce a template unit type by converting to a `per`-of-`by`s and
    // cancelling units.
    
    template< template< typename > class, typename = void > struct reduce;
    
    template< template< typename > class Unit > struct reduce<
        Unit,
        typename std::enable_if<
            is_basic_unit< Unit< void > >::value
            ||   is_ratio< Unit< void > >::value
        >::type
    >
    {
        template< typename T > using unit_type = Unit< T >;
    };
    
    template< template< typename > class Unit > struct reduce<
        Unit,
        typename std::enable_if<
              is_per< Unit< void > >::value
            || is_by< Unit< void > >::value
        >::type
    >
    {
        using _simplified = simplify<
            typename take_numer< Unit >::type,
            typename take_denom< Unit >::type
        >;
        
        template< typename T > using unit_type = typename unit_lists_to_unit<
            typename _simplified:: first_list,
            typename _simplified::second_list
        >::template unit_type< T >;
    };
    
    // Shortcut aliased to the reduced version of T
    template< typename T > using reduced = typename reduce<
        T::template unit_type
    >::template unit_type< typename T::value_type >;
} } }


#endif
