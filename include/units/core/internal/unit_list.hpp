#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_UNIT_LIST_HPP
#define JM_UNITS_CORE_INTERNAL_UNIT_LIST_HPP


#include "core_type_detection.hpp"
#include "core_types.hpp"

#include <ratio>
#include <type_traits>  // conditional


namespace JadeMatrix { namespace units { namespace internal // Unit list ///////
{
    // Represents a recursible list of unit template types
    
    template< template< typename > class... > struct unit_list;
    
    template<
        template< typename > class FirstUnit,
        template< typename > class... RestUnits
    > struct unit_list< FirstUnit, RestUnits... >
    {
        template< typename T > using first_unit = FirstUnit< T >;
        using rest_type = unit_list< RestUnits... >;
    };
    
    template<> struct unit_list<> {};
    
    template< typename, typename > struct unit_list_cat;
    template<
        template< typename > class...  FirstUnits,
        template< typename > class... SecondUnits
    > struct unit_list_cat<
        unit_list<  FirstUnits... >,
        unit_list< SecondUnits... >
    >
    {
        using type = unit_list< FirstUnits..., SecondUnits... >;
    };
} } }


namespace JadeMatrix { namespace units { namespace internal // Take ////////////
{
    // Separate unit template types into numer/denom lists
    
    template< template< typename > class, typename = void > struct take_numer;
    template< template< typename > class, typename = void > struct take_denom;
    
    // Specializations for `unit`
    template< template< typename > class Unit > struct take_numer<
        Unit,
        typename std::enable_if< is_basic_unit< Unit< void > >::value >::type
    >
    {
        using type = unit_list< Unit >;
    };
    template< template< typename > class Unit > struct take_denom<
        Unit,
        typename std::enable_if< is_basic_unit< Unit< void > >::value >::type
    >
    {
        using type = unit_list<>;
    };
    
    // Specializations for `ratio`
    template< template< typename > class Unit > struct take_numer<
        Unit,
        typename std::enable_if< is_ratio< Unit< void > >::value >::type
    >
    {
        using type = unit_list<>;
    };
    template< template< typename > class Unit > struct take_denom<
        Unit,
        typename std::enable_if< is_ratio< Unit< void > >::value >::type
    >
    {
        using type = unit_list<>;
    };
    
    // Specializations for `per`
    template< template< typename > class Unit > struct take_numer<
        Unit,
        typename std::enable_if< is_per< Unit< void > >::value >::type
    >
    {
        using type = typename unit_list_cat<
            typename take_numer< Unit< void >::template numer_unit >::type,
            typename take_denom< Unit< void >::template denom_unit >::type
        >::type;
    };
    template< template< typename > class Unit > struct take_denom<
        Unit,
        typename std::enable_if< is_per< Unit< void > >::value >::type
    >
    {
        using type = typename unit_list_cat<
            typename take_denom< Unit< void >::template numer_unit >::type,
            typename take_numer< Unit< void >::template denom_unit >::type
        >::type;
    };
    
    // Specializations for `by`
    template< template< typename > class Unit > struct take_numer<
        Unit,
        typename std::enable_if< is_by< Unit< void > >::value >::type
    >
    {
        using type = typename unit_list_cat<
            typename take_numer< Unit< void >::template  first_unit >::type,
            typename take_numer< Unit< void >::template second_unit >::type
        >::type;
    };
    template< template< typename > class Unit > struct take_denom<
        Unit,
        typename std::enable_if< is_by< Unit< void > >::value >::type
    >
    {
        using type = typename unit_list_cat<
            typename take_denom< Unit< void >::template  first_unit >::type,
            typename take_denom< Unit< void >::template second_unit >::type
        >::type;
    };
} } }


namespace JadeMatrix { namespace units { namespace internal // Remove from list
{
    // Remove a template unit type from a unit list if it occurs in that list.
    // Static member value `removed` signals if the template type was removed
    // from the list.  Member `type` is an alias to the (potentially) modified
    // unit list.
    
    // Reduction comparison that simply checks that two units are the same
    template<
        template< typename > class  FirstUnit,
        template< typename > class SecondUnit
    > struct compare_is_same : is_same_unit< FirstUnit, SecondUnit >
    {
        using additional_scale = std::ratio< 1 >;
    };
    
    template<
        template< typename > class Unit,
        typename List,
        template<
            template< typename > class,
            template< typename > class
        > class Compare = compare_is_same
    > struct remove_from_list;
    
    template<
        template< typename > class Unit,
        template< typename > class   FirstInList,
        template< typename > class... RestInList,
        template<
            template< typename > class,
            template< typename > class
        > class Compare
    > struct remove_from_list<
        Unit,
        unit_list< FirstInList, RestInList... >,
        Compare
    >
    {
        using _compare = Compare<
            Unit,
            FirstInList
        >;
        using _remove_from_rest = remove_from_list<
            Unit,
            unit_list< RestInList... >,
            Compare
        >;
        
        using additional_scale = typename std::conditional<
            _compare::value,
            typename _compare         ::additional_scale,
            typename _remove_from_rest::additional_scale
        >::type;
        
        using type = typename std::conditional<
            _compare::value,
            unit_list< RestInList... >,
            typename unit_list_cat<
                unit_list< FirstInList >,
                typename _remove_from_rest::type
            >::type
        >::type;
        static constexpr auto removed = (
            _compare::value
            || _remove_from_rest::removed
        );
    };
    
    template<
        template< typename > class Unit,
        template<
            template< typename > class,
            template< typename > class
        > class Compare
    > struct remove_from_list< Unit, unit_list<>, Compare >
    {
        using type = unit_list<>;
        static constexpr auto removed = false;
        using additional_scale = std::ratio< 1 >;
    };
} } }


namespace JadeMatrix { namespace units { namespace internal // Simplify lists //
{
    // Remove pairs of duplicates across two unit lists, like units on either
    // side of a division operation cancel out.  For example, given the two unit
    // lists:
    //      (a, b, c, b)    and     (b)
    // the resulting pair of lists would be
    //      (a, c, b)       and     ()
    // Members `first_list` and `second_list` are aliased to the two
    // (potentially) modified lists, respectively.
    
    template<
        typename NumerList,
        typename DenomList,
        template<
            template< typename > class,
            template< typename > class
        > class Compare = compare_is_same
    > struct simplify;
    
    template<
        template< typename > class FirstFirstUnit,
        template< typename > class... FirstRestUnits,
        template< typename > class... SecondUnits,
        template<
            template< typename > class,
            template< typename > class
        > class Compare
    > struct simplify<
        unit_list< FirstFirstUnit, FirstRestUnits... >,
        unit_list< SecondUnits... >,
        Compare
    >
    {
        using _removal = remove_from_list<
            FirstFirstUnit,
            unit_list< SecondUnits... >,
            Compare
        >;
        using _simplify_rest = simplify<
            unit_list< FirstRestUnits... >,
            typename std::conditional<
                _removal::removed,
                typename _removal::type,
                unit_list< SecondUnits... >
            >::type,
            Compare
        >;
        
        using first_list = typename std::conditional<
            _removal::removed,
            typename _simplify_rest::first_list,
            typename unit_list_cat<
                unit_list< FirstFirstUnit >,
                typename _simplify_rest::first_list
            >::type
        >::type;
        using second_list = typename _simplify_rest::second_list;
        using additional_scale = std::ratio_multiply<
            typename std::conditional<
                _removal::removed,
                typename _removal::additional_scale,
                std::ratio< 1 >
            >::type,
            typename _simplify_rest::additional_scale
        >;
    };
    
    template<
        template< typename > class SecondFirstUnit,
        template< typename > class... SecondRestUnits,
        template<
            template< typename > class,
            template< typename > class
        > class Compare
    > struct simplify<
        unit_list<>,
        unit_list< SecondFirstUnit, SecondRestUnits... >,
        Compare
    >
    {
        using  first_list = unit_list<>;
        using second_list = unit_list< SecondFirstUnit, SecondRestUnits... >;
        using additional_scale = std::ratio< 1 >;
    };
    
    template<
        template< typename > class FirstFirstUnit,
        template< typename > class... FirstRestUnits,
        template<
            template< typename > class,
            template< typename > class
        > class Compare
    > struct simplify<
        unit_list< FirstFirstUnit, FirstRestUnits... >,
        unit_list<>,
        Compare
    >
    {
        using  first_list = unit_list< FirstFirstUnit, FirstRestUnits... >;
        using second_list = unit_list<>;
        using additional_scale = std::ratio< 1 >;
    };
    
    template<
        template<
            template< typename > class,
            template< typename > class
        > class Compare
    > struct simplify<
        unit_list<>,
        unit_list<>,
        Compare
    >
    {
        using  first_list = unit_list<>;
        using second_list = unit_list<>;
        using additional_scale = std::ratio< 1 >;
    };
} } }


namespace JadeMatrix { namespace units { namespace internal // List to unit ////
{
    // Convert a unit list to an appropriate template unit type.  A list of two
    // or more template unit types becomes a `by`, a list of one becomes that
    // that template type, and a list of none becomes a `ratio`.  The template
    // member `unit_type` is aliased to the resulting unit.
    
    template< typename > struct unit_list_to_unit;
    
    template<
        template< typename > class   FirstUnit,
        template< typename > class  SecondUnit,
        template< typename > class... RestUnits
    > struct unit_list_to_unit<
        unit_list< FirstUnit, SecondUnit, RestUnits... >
    >
    {
        template< typename T > using unit_type = by<
            FirstUnit,
            unit_list_to_unit<
                unit_list< SecondUnit, RestUnits... >
            >::template unit_type,
            T
        >;
    };
    
    template<
        template< typename > class Unit
    > struct unit_list_to_unit<
        unit_list< Unit >
    >
    {
        template< typename T > using unit_type = Unit< T >;
    };
    
    template<> struct unit_list_to_unit< unit_list<> >
    {
        template< typename T > using unit_type = ratio< T >;
    };
} } }


namespace JadeMatrix { namespace units { namespace internal // List pair to unit
{
    // Convert a numer/denom unit list pair into an appropriate template unit
    // type.
    
    template< typename, typename, typename = void > struct unit_lists_to_unit;
    
    template<
        template< typename > class... NumerUnits,
        template< typename > class DenomFirstUnit,
        template< typename > class... DenomRestUnits
    > struct unit_lists_to_unit<
        unit_list< NumerUnits... >,
        unit_list< DenomFirstUnit, DenomRestUnits... >
    >
    {
        template< typename T > using _numer_unit = typename unit_list_to_unit<
            unit_list< NumerUnits... >
        >::template unit_type< T >;
        template< typename T > using _denom_unit = typename unit_list_to_unit<
            unit_list< DenomFirstUnit, DenomRestUnits... >
        >::template unit_type< T >;
        
        template< typename T > using unit_type = per<
            _numer_unit,
            _denom_unit,
            T
        >;
    };
    
    template<
        template< typename > class... Units
    > struct unit_lists_to_unit<
        unit_list< Units... >,
        unit_list<>
    >
    {
        template< typename T > using unit_type = typename unit_list_to_unit<
            unit_list< Units... >
        >::template unit_type< T >;
    };
} } }


#endif
