#pragma once
#ifndef JM_UNITS_STRINGS_HPP
#define JM_UNITS_STRINGS_HPP


#include "internal/traits_utils.hpp"

#include <string>


namespace JadeMatrix { namespace units
{
    template< typename Unit, typename = void > struct unit_strings;
    
    #define DEFINE_NAME_SYM_STRINGS_FOR_UNIT( \
        UNIT_PLURAL, \
        SCALE_NAME, \
        UNIT_NAME_STR, \
        UNIT_SYM_STR, \
        SCALE_PREFIX_STR, \
        SCALE_SYM_PREFIX_STR, \
        SCALE_SYM_SUFFIX_STR \
    ) \
    template< typename Unit > struct unit_strings< \
        Unit, \
        typename std::enable_if< std::is_same< \
            Unit, \
            SCALE_NAME##UNIT_PLURAL< typename Unit::value_type > \
        >::value >::type \
    > \
    { \
        static const std::string& name() \
        { \
            static const std::string s{ \
                  std::string{ SCALE_PREFIX_STR } \
                + std::string{ UNIT_NAME_STR    } \
            }; \
            return s; \
        } \
        static const std::string& symbol() \
        { \
            static const std::string s{ \
                  std::string{ SCALE_SYM_PREFIX_STR } \
                + std::string{ UNIT_SYM_STR         } \
                + std::string{ SCALE_SYM_SUFFIX_STR } \
            }; \
            return s; \
        } \
    };
    
    #define DEFINE_ALL_STRINGS_FOR_UNIT( \
        UNIT_PLURAL, \
        UNIT_NAME_STR, \
        UNIT_SYM_STR \
    ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, exa   , UNIT_NAME_STR, UNIT_SYM_STR, "exa"   , "E"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, peta  , UNIT_NAME_STR, UNIT_SYM_STR, "peta"  , "P"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, tera  , UNIT_NAME_STR, UNIT_SYM_STR, "tera"  , "T"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, giga  , UNIT_NAME_STR, UNIT_SYM_STR, "giga"  , "G"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, mega  , UNIT_NAME_STR, UNIT_SYM_STR, "mega"  , "M"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, kilo  , UNIT_NAME_STR, UNIT_SYM_STR, "kilo"  , "k"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, hecto , UNIT_NAME_STR, UNIT_SYM_STR, "hecto" , "h"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, deca  , UNIT_NAME_STR, UNIT_SYM_STR, "deca"  , "da"  , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL,       , UNIT_NAME_STR, UNIT_SYM_STR, ""      , ""    , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, deci  , UNIT_NAME_STR, UNIT_SYM_STR, "deci"  , "d"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, centi , UNIT_NAME_STR, UNIT_SYM_STR, "centi" , "c"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, milli , UNIT_NAME_STR, UNIT_SYM_STR, "milli" , "m"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, micro , UNIT_NAME_STR, UNIT_SYM_STR, "micro" , "μ"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, nano  , UNIT_NAME_STR, UNIT_SYM_STR, "nano"  , "n"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, pico  , UNIT_NAME_STR, UNIT_SYM_STR, "pico"  , "p"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, femto , UNIT_NAME_STR, UNIT_SYM_STR, "femto" , "f"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, atto  , UNIT_NAME_STR, UNIT_SYM_STR, "atto"  , "a"   , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL, dozen_, UNIT_NAME_STR, UNIT_SYM_STR, "dozen ", "dz." , ""   ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL,    bi , UNIT_NAME_STR, UNIT_SYM_STR, "bi"    , ""    , "/2" ) \
    DEFINE_NAME_SYM_STRINGS_FOR_UNIT( UNIT_PLURAL,  semi , UNIT_NAME_STR, UNIT_SYM_STR, "semi"  , ""    , "*2" )
    
    template< typename Unit > struct unit_strings<
        Unit,
        typename std::enable_if< is_per< Unit >::value >::type
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
        typename std::enable_if< is_by< Unit >::value >::type
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
        typename std::enable_if< is_ratio< Unit >::value >::type
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
