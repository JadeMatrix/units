#pragma once
#ifndef JM_UNITS_DIGITAL_HPP
#define JM_UNITS_DIGITAL_HPP


// #include "core/constants.hpp"
#include "core/define_unit.hpp"


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
    
    struct  bit_traits {};
    struct byte_traits {};
    struct flop_traits {};
    struct   ip_traits {}; // Instructions-Per-Second, not Internet Protocol
    
    DEFINE_ALL_PREFIXES_FOR_UNIT(  bits,  bit_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT( bytes, byte_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT( flops, flop_traits )
    DEFINE_ALL_PREFIXES_FOR_UNIT(   ips,   ip_traits )
    
    DEFINE_ALL_STRINGS_FOR_UNIT(  bit_traits,  "bits",     "b" )
    DEFINE_ALL_STRINGS_FOR_UNIT( byte_traits, "bytes",     "B" )
    DEFINE_ALL_STRINGS_FOR_UNIT( flop_traits, "FLOPS", "FLOPS" )
    DEFINE_ALL_STRINGS_FOR_UNIT(   ip_traits,   "IPS",   "IPS" )
} }


#endif
