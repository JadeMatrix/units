#pragma once
#ifndef JM_UNITS_CORE_SCALES_HPP
#define JM_UNITS_CORE_SCALES_HPP


#include <cstdint>  // INTMAX_MAX
#include <ratio>


#if INTMAX_MAX / 1000000000000000000 >= 1000
    #define JM_UNITS_ZT_AVAILABLE
#endif
#if INTMAX_MAX / 1000000000000000000 >= 1000000
    #define JM_UNITS_YT_AVAILABLE
#endif


namespace JadeMatrix { namespace units
{
    using  unit_scale = std::ratio<  1, 1 >;
    using dozen_scale = std::ratio< 12, 1 >;
    using    bi_scale = std::ratio<  2, 1 >;
    using  semi_scale = std::ratio<  1, 2 >;
    
    using   exa_scale = std::  exa;
    using  peta_scale = std:: peta;
    using  tera_scale = std:: tera;
    using  giga_scale = std:: giga;
    using  mega_scale = std:: mega;
    using  kilo_scale = std:: kilo;
    using hecto_scale = std::hecto;
    using  deca_scale = std:: deca;
    
    using  deci_scale = std:: deci;
    using centi_scale = std::centi;
    using milli_scale = std::milli;
    using micro_scale = std::micro;
    using  nano_scale = std:: nano;
    using  pico_scale = std:: pico;
    using femto_scale = std::femto;
    using  atto_scale = std:: atto;
    
    using exbi_scale = std::ratio< ( 2L << ( 60 - 1 ) ), 1 >;
    using pebi_scale = std::ratio< ( 2L << ( 50 - 1 ) ), 1 >;
    using tebi_scale = std::ratio< ( 2L << ( 40 - 1 ) ), 1 >;
    using gibi_scale = std::ratio< ( 2L << ( 30 - 1 ) ), 1 >;
    using mebi_scale = std::ratio< ( 2L << ( 20 - 1 ) ), 1 >;
    using kibi_scale = std::ratio< ( 2L << ( 10 - 1 ) ), 1 >;
    
    #if defined JM_UNITS_ZT_AVAILABLE
    using zepto_scale = std::zepto;
    using zetta_scale = std::zetta;
    using  zebi_scale = std::ratio< ( 2L << ( 70 - 1 ) ), 1 >;
    #endif

    #if defined JM_UNITS_YT_AVAILABLE
    using yocto_scale = std::yocto;
    using yotta_scale = std::yotta;
    using  yobi_scale = std::ratio< ( 2L << ( 80 - 1 ) ), 1 >;
    #endif
} }


#endif
