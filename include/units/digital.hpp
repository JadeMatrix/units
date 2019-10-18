#pragma once
#ifndef JM_UNITS_DIGITAL_HPP
#define JM_UNITS_DIGITAL_HPP


#include "core/define_unit.hpp"
#include "temporal.hpp"


namespace JadeMatrix { namespace units // Declarations /////////////////////////
{
    /*
    No automatic conversion is given between `bits` and `bytes` because:
        - Not all computers use 8-bit bytes.  While most of the machines that
          don't are only of historical interest, they still exist or did exist.
          Besides that...
        - There is no way of knowing the user's intention in using `bits`.  They
          may be using it to store the compressed size of some data that may not
          have a direct relationship to the bytes in which it is stored.  For
          example, a function to count the number of bits set to 1 in a word
          could return a value as `bits` which would not be meaningful as some
          number of `bytes`.
    */
    
    struct         bit_traits {};
    struct        byte_traits {};
    struct     floatop_traits {};
    struct instruction_traits {};
    
    
    #define DEFINE_PREFIX_FOR_bits(         PREFIX, SCALE ) template< typename T > using PREFIX##bits         = unit<         bit_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_bytes(        PREFIX, SCALE ) template< typename T > using PREFIX##bytes        = unit<        byte_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_floatops(     PREFIX, SCALE ) template< typename T > using PREFIX##floatops     = unit<     floatop_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_instructions( PREFIX, SCALE ) template< typename T > using PREFIX##instructions = unit< instruction_traits, SCALE, T >;
    #define DEFINE_PREFIX_FOR_flops(        PREFIX, SCALE ) template< typename T > using PREFIX##flops        = per< PREFIX##floatops    , seconds, T >;
    #define DEFINE_PREFIX_FOR_ips(          PREFIX, SCALE ) template< typename T > using PREFIX##ips          = per< PREFIX##instructions, seconds, T >;
    
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_bits         )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_bytes        )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_floatops     )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_instructions )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_flops        )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_ips          )
    
    #undef DEFINE_PREFIX_FOR_bits
    #undef DEFINE_PREFIX_FOR_bytes
    #undef DEFINE_PREFIX_FOR_floatops
    #undef DEFINE_PREFIX_FOR_instructions
    #undef DEFINE_PREFIX_FOR_flops
    #undef DEFINE_PREFIX_FOR_ips
    
    
    DEFINE_ALL_STRINGS_FOR_UNIT(         bit_traits,         "bits",     "b" )
    DEFINE_ALL_STRINGS_FOR_UNIT(        byte_traits,        "bytes",     "B" )
    DEFINE_ALL_STRINGS_FOR_UNIT(     floatop_traits,     "floatops", "FLOPs" )
    DEFINE_ALL_STRINGS_FOR_UNIT( instruction_traits, "instructions",  "inst" )
    
    
    template<> struct unit_strings< flops< void > >
    {
        static const std::string& name()
        {
            static const std::string s{ "FLOPS" };
            return s;
        }
        static const std::string& symbol()
        {
            static const std::string s{ "FLOPS" };
            return s;
        }
    };
    template<> struct unit_strings< ips< void > >
    {
        static const std::string& name()
        {
            static const std::string s{ "IPS" };
            return s;
        }
        static const std::string& symbol()
        {
            static const std::string s{ "IPS" };
            return s;
        }
    };
} }


#endif
