#pragma once
#ifndef JM_UNITS_CORE_STRINGS_HPP
#define JM_UNITS_CORE_STRINGS_HPP


#include "scales.hpp"
#include "internal/core_type_detection.hpp"

#include <string>
#include <type_traits>  // enable_if
#include <utility>      // declval


namespace JadeMatrix { namespace units
{
    template< typename Unit, typename = void > struct  unit_strings;
    template< typename Scale                 > struct scale_strings;
    
    #define DEFINE_STRINGS_FOR_SCALE( \
        SCALE_NAME, \
        SCALE_PREFIX_STR, \
        SCALE_SYM_PREFIX_STR, \
        SCALE_SYM_SUFFIX_STR \
    ) \
    template<> struct scale_strings< SCALE_NAME##_scale > \
    { \
        static const std::string& prefix() \
        { \
            static const std::string s{ SCALE_PREFIX_STR }; \
            return s; \
        } \
        static const std::string& prefix_symbol() \
        { \
            static const std::string s{ SCALE_SYM_PREFIX_STR }; \
            return s; \
        } \
        static const std::string& suffix_symbol() \
        { \
            static const std::string s{ SCALE_SYM_SUFFIX_STR }; \
            return s; \
        } \
    };
    
    DEFINE_STRINGS_FOR_SCALE( unit , ""      , ""    , ""   )
    DEFINE_STRINGS_FOR_SCALE( dozen, "dozen ", "dz." , ""   )
    DEFINE_STRINGS_FOR_SCALE( bi   , "bi"    , ""    , "/2" )
    DEFINE_STRINGS_FOR_SCALE( semi , "semi"  , ""    , "*2" )
    
    DEFINE_STRINGS_FOR_SCALE( exa  , "exa"   , "E"   , ""   )
    DEFINE_STRINGS_FOR_SCALE( peta , "peta"  , "P"   , ""   )
    DEFINE_STRINGS_FOR_SCALE( tera , "tera"  , "T"   , ""   )
    DEFINE_STRINGS_FOR_SCALE( giga , "giga"  , "G"   , ""   )
    DEFINE_STRINGS_FOR_SCALE( mega , "mega"  , "M"   , ""   )
    DEFINE_STRINGS_FOR_SCALE( kilo , "kilo"  , "k"   , ""   )
    DEFINE_STRINGS_FOR_SCALE( hecto, "hecto" , "h"   , ""   )
    DEFINE_STRINGS_FOR_SCALE( deca , "deca"  , "da"  , ""   )
    
    DEFINE_STRINGS_FOR_SCALE( deci , "deci"  , "d"   , ""   )
    DEFINE_STRINGS_FOR_SCALE( centi, "centi" , "c"   , ""   )
    DEFINE_STRINGS_FOR_SCALE( milli, "milli" , "m"   , ""   )
    DEFINE_STRINGS_FOR_SCALE( micro, "micro" , "μ"   , ""   )
    DEFINE_STRINGS_FOR_SCALE( nano , "nano"  , "n"   , ""   )
    DEFINE_STRINGS_FOR_SCALE( pico , "pico"  , "p"   , ""   )
    DEFINE_STRINGS_FOR_SCALE( femto, "femto" , "f"   , ""   )
    DEFINE_STRINGS_FOR_SCALE( atto , "atto"  , "a"   , ""   )
    
    DEFINE_STRINGS_FOR_SCALE( exbi , "exbi"  , "Ei"  , ""   )
    DEFINE_STRINGS_FOR_SCALE( pebi , "pebi"  , "Pi"  , ""   )
    DEFINE_STRINGS_FOR_SCALE( tebi , "tebi"  , "Ti"  , ""   )
    DEFINE_STRINGS_FOR_SCALE( gibi , "gibi"  , "Gi"  , ""   )
    DEFINE_STRINGS_FOR_SCALE( mebi , "mebi"  , "Mi"  , ""   )
    DEFINE_STRINGS_FOR_SCALE( kibi , "kibi"  , "Ki"  , ""   )
    
    #if defined JM_UNITS_ZT_AVAILABLE
    DEFINE_STRINGS_FOR_SCALE( zepto, "zepto", "z" , "" )
    DEFINE_STRINGS_FOR_SCALE( zetta, "zetta", "Z" , "" )
    DEFINE_STRINGS_FOR_SCALE( zebi , "zebi" , "Zi", "" )
    #endif

    #if defined JM_UNITS_YT_AVAILABLE
    DEFINE_STRINGS_FOR_SCALE( yocto, "yocto", "y" , "" )
    DEFINE_STRINGS_FOR_SCALE( yotta, "yotta", "Y" , "" )
    DEFINE_STRINGS_FOR_SCALE( yobi , "yobi" , "Yi", "" )
    #endif
    
    #undef DEFINE_STRINGS_FOR_SCALE
    
    template< typename Unit > struct unit_strings<
        Unit,
        typename std::enable_if< internal::is_basic_unit< Unit >::value >::type
    >
    {
        // ADL on traits type
        using unit_trait_strings = decltype( units_unit_strings_lookup(
            std::declval< typename Unit::traits_type >()
        ) );
        
        static const std::string& name()
        {
            static const std::string s{
                  scale_strings< typename Unit::scale_type >::prefix()
                + unit_trait_strings::unit_name()
            };
            return s;
        }
        static const std::string& symbol()
        {
            static const std::string s{
                  scale_strings< typename Unit::scale_type >::prefix_symbol()
                + unit_trait_strings::unit_symbol()
                + scale_strings< typename Unit::scale_type >::suffix_symbol()
            };
            return s;
        }
    };
    
    template< typename Unit > struct unit_strings<
        Unit,
        typename std::enable_if< internal::is_per< Unit >::value >::type
    >
    {
        static const std::string& name()
        {
            static const std::string s{
                "("
                + unit_strings<
                    typename Unit::template numer_unit< void >
                >::name()
                + ")/("
                + unit_strings<
                    typename Unit::template denom_unit< void >
                >::name()
                + ")"
            };
            return s;
        }
        static const std::string& symbol()
        {
            static const std::string s{
                "("
                + unit_strings<
                    typename Unit::template numer_unit< void >
                >::symbol()
                + ")/("
                + unit_strings<
                    typename Unit::template denom_unit< void >
                >::symbol()
                + ")"
            };
            return s;
        }
    };
    
    template< typename Unit > struct unit_strings<
        Unit,
        typename std::enable_if< internal::is_by< Unit >::value >::type
    >
    {
        static const std::string& name()
        {
            static const std::string s{
                "("
                + unit_strings<
                    typename Unit::template first_unit< void >
                >::name()
                + ")*("
                + unit_strings<
                    typename Unit::template second_unit< void >
                >::name()
                + ")"
            };
            return s;
        }
        static const std::string& symbol()
        {
            static const std::string s{
                "("
                + unit_strings<
                    typename Unit::template first_unit< void >
                >::symbol()
                + ")*("
                + unit_strings<
                    typename Unit::template second_unit< void >
                >::symbol()
                + ")"
            };
            return s;
        }
    };
    
    template< typename Unit > struct unit_strings<
        Unit,
        typename std::enable_if< internal::is_ratio< Unit >::value >::type
    >
    {
        static const std::string& name()
        {
            static const std::string s{ "ratio" };
            return s;
        }
        static const std::string& symbol()
        {
            static const std::string s{};
            return s;
        }
    };
} }


#endif
