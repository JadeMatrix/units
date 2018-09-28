#pragma once
#ifndef JM_UNITS_SCALES_HPP
#define JM_UNITS_SCALES_HPP


#include <string>


namespace JadeMatrix { namespace units
{
    #define DEFINE_SCALE_SPECIALIZATION( \
        NAME, \
        PREFIX, \
        SYM_PREFIX, \
        SYM_SUFFIX, \
        FACTOR, \
        OPERAND \
    ) \
    struct NAME##_scale \
    { \
        template< typename T > \
        static constexpr T magnitude() { return static_cast< T >( FACTOR ); } \
         \
        template< typename T > static constexpr auto scale( const T& v ) \
            -> decltype( v OPERAND magnitude< T >() ) \
        { \
            return v OPERAND magnitude< T >(); \
        } \
        template< typename T > static constexpr auto unscale( const T& v ) \
            -> decltype( v / ( 1 OPERAND magnitude< T >() ) ) \
        { \
            return v / ( 1 OPERAND magnitude< T >() ); \
        } \
    };
    
    DEFINE_SCALE_SPECIALIZATION( exa  , "exa"   , "E"   , ""  , 1000000000000000000, / )
    DEFINE_SCALE_SPECIALIZATION( peta , "peta"  , "P"   , ""  , 1000000000000000   , / )
    DEFINE_SCALE_SPECIALIZATION( tera , "tera"  , "T"   , ""  , 1000000000000      , / )
    DEFINE_SCALE_SPECIALIZATION( giga , "giga"  , "G"   , ""  , 1000000000         , / )
    DEFINE_SCALE_SPECIALIZATION( mega , "mega"  , "M"   , ""  , 1000000            , / )
    DEFINE_SCALE_SPECIALIZATION( kilo , "kilo"  , "k"   , ""  , 1000               , / )
    DEFINE_SCALE_SPECIALIZATION( hecto, "hecto" , "h"   , ""  , 100                , / )
    DEFINE_SCALE_SPECIALIZATION( deca , "deca"  , "da"  , ""  , 10                 , / )
    
    DEFINE_SCALE_SPECIALIZATION( unit , ""      , ""    , ""  , 1                  , * )
    
    DEFINE_SCALE_SPECIALIZATION( deci , "deci"  , "d"   , ""  , 10                 , * )
    DEFINE_SCALE_SPECIALIZATION( centi, "centi" , "c"   , ""  , 100                , * )
    DEFINE_SCALE_SPECIALIZATION( milli, "milli" , "m"   , ""  , 1000               , * )
    DEFINE_SCALE_SPECIALIZATION( micro, "micro" , "μ"   , ""  , 1000000            , * )
    DEFINE_SCALE_SPECIALIZATION( nano , "nano"  , "n"   , ""  , 1000000000         , * )
    DEFINE_SCALE_SPECIALIZATION( pico , "pico"  , "p"   , ""  , 1000000000000      , * )
    DEFINE_SCALE_SPECIALIZATION( femto, "femto" , "f"   , ""  , 1000000000000000   , * )
    DEFINE_SCALE_SPECIALIZATION( atto , "atto"  , "a"   , ""  , 1000000000000000000, * )
    
    DEFINE_SCALE_SPECIALIZATION( dozen, "dozen ", "dz." , ""  , 12                 , / )
    DEFINE_SCALE_SPECIALIZATION(    bi, "bi"    , ""    , "/2",  2                 , / )
    DEFINE_SCALE_SPECIALIZATION(  semi, "semi"  , ""    , "*2",  2                 , * )
    
    #undef DEFINE_SCALE_SPECIALIZATION
} }


namespace JadeMatrix { namespace units // Helper for defining prefixed units ///
{
    #define DEFINE_ALL_PREFIXES_FOR_UNIT( PLURAL, TRAITS ) \
    template< typename T > using    exa##PLURAL = unit< TRAITS, T, ::JadeMatrix::units::  exa_scale >; \
    template< typename T > using   peta##PLURAL = unit< TRAITS, T, ::JadeMatrix::units:: peta_scale >; \
    template< typename T > using   tera##PLURAL = unit< TRAITS, T, ::JadeMatrix::units:: tera_scale >; \
    template< typename T > using   giga##PLURAL = unit< TRAITS, T, ::JadeMatrix::units:: giga_scale >; \
    template< typename T > using   mega##PLURAL = unit< TRAITS, T, ::JadeMatrix::units:: mega_scale >; \
    template< typename T > using   kilo##PLURAL = unit< TRAITS, T, ::JadeMatrix::units:: kilo_scale >; \
    template< typename T > using  hecto##PLURAL = unit< TRAITS, T, ::JadeMatrix::units::hecto_scale >; \
    template< typename T > using   deca##PLURAL = unit< TRAITS, T, ::JadeMatrix::units:: deca_scale >; \
     \
    template< typename T > using         PLURAL = unit< TRAITS, T, ::JadeMatrix::units:: unit_scale >; \
     \
    template< typename T > using   deci##PLURAL = unit< TRAITS, T, ::JadeMatrix::units:: deci_scale >; \
    template< typename T > using  centi##PLURAL = unit< TRAITS, T, ::JadeMatrix::units::centi_scale >; \
    template< typename T > using  milli##PLURAL = unit< TRAITS, T, ::JadeMatrix::units::milli_scale >; \
    template< typename T > using  micro##PLURAL = unit< TRAITS, T, ::JadeMatrix::units::micro_scale >; \
    template< typename T > using   nano##PLURAL = unit< TRAITS, T, ::JadeMatrix::units:: nano_scale >; \
    template< typename T > using   pico##PLURAL = unit< TRAITS, T, ::JadeMatrix::units:: pico_scale >; \
    template< typename T > using  femto##PLURAL = unit< TRAITS, T, ::JadeMatrix::units::femto_scale >; \
    template< typename T > using   atto##PLURAL = unit< TRAITS, T, ::JadeMatrix::units:: atto_scale >; \
     \
    template< typename T > using dozen_##PLURAL = unit< TRAITS, T, ::JadeMatrix::units::dozen_scale >; \
    template< typename T > using     bi##PLURAL = unit< TRAITS, T, ::JadeMatrix::units::   bi_scale >; \
    template< typename T > using   semi##PLURAL = unit< TRAITS, T, ::JadeMatrix::units:: semi_scale >;
} }


#endif