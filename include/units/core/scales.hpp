#pragma once
#ifndef JM_UNITS_CORE_SCALES_HPP
#define JM_UNITS_CORE_SCALES_HPP


#include <cstdint>
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
    
    #if defined JM_UNITS_ZT_AVAILABLE
    using zepto_scale = std::zepto;
    using zetta_scale = std::zetta;
    #endif

    #if defined JM_UNITS_YT_AVAILABLE
    using yocto_scale = std::yocto;
    using yotta_scale = std::yotta;
    #endif
} }


namespace JadeMatrix { namespace units // Helper for defining prefixed units ///
{
    #define _DEFINE_MOST_PREFIXES_FOR_UNIT( PLURAL, TRAITS ) \
    template< typename T > using         PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units:: unit_scale, T >; \
    template< typename T > using dozen_##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units::dozen_scale, T >; \
    template< typename T > using     bi##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units::   bi_scale, T >; \
    template< typename T > using   semi##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units:: semi_scale, T >; \
     \
    template< typename T > using    exa##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units::  exa_scale, T >; \
    template< typename T > using   peta##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units:: peta_scale, T >; \
    template< typename T > using   tera##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units:: tera_scale, T >; \
    template< typename T > using   giga##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units:: giga_scale, T >; \
    template< typename T > using   mega##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units:: mega_scale, T >; \
    template< typename T > using   kilo##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units:: kilo_scale, T >; \
    template< typename T > using  hecto##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units::hecto_scale, T >; \
    template< typename T > using   deca##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units:: deca_scale, T >; \
     \
    template< typename T > using   deci##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units:: deci_scale, T >; \
    template< typename T > using  centi##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units::centi_scale, T >; \
    template< typename T > using  milli##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units::milli_scale, T >; \
    template< typename T > using  micro##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units::micro_scale, T >; \
    template< typename T > using   nano##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units:: nano_scale, T >; \
    template< typename T > using   pico##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units:: pico_scale, T >; \
    template< typename T > using  femto##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units::femto_scale, T >; \
    template< typename T > using   atto##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units:: atto_scale, T >;
    
    #if defined JM_UNITS_YT_AVAILABLE
        #define DEFINE_ALL_PREFIXES_FOR_UNIT( PLURAL, TRAITS ) \
        _DEFINE_MOST_PREFIXES_FOR_UNIT( PLURAL, TRAITS ) \
        template< typename T > using zepto##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units::zepto_scale, T >; \
        template< typename T > using zetta##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units::zetta_scale, T >; \
        template< typename T > using yocto##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units::yocto_scale, T >; \
        template< typename T > using yotta##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units::yotta_scale, T >;
    #elif defined JM_UNITS_ZT_AVAILABLE
        #define DEFINE_ALL_PREFIXES_FOR_UNIT( PLURAL, TRAITS ) \
        _DEFINE_MOST_PREFIXES_FOR_UNIT( PLURAL, TRAITS ) \
        template< typename T > using zepto##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units::zepto_scale, T >; \
        template< typename T > using zetta##PLURAL = ::JadeMatrix::units::unit< TRAITS, ::JadeMatrix::units::zetta_scale, T >;
    #else
        #define DEFINE_ALL_PREFIXES_FOR_UNIT( PLURAL, TRAITS ) \
        _DEFINE_MOST_PREFIXES_FOR_UNIT( PLURAL, TRAITS )
    #endif
} }


#endif
