#pragma once
#ifndef JM_UNITS_PREFIXES_HPP
#define JM_UNITS_PREFIXES_HPP


#include "unit.hpp"

#include <string>


namespace JadeMatrix { namespace units // Declarations /////////////////////////
{
    template< typename T, class Base_Traits, long long Factor > struct mul_prefix_traits;
    template< typename T, class Base_Traits, long long Factor > struct div_prefix_traits;
    
    
    template< typename T, class Base_Traits > using   exa_traits = mul_prefix_traits< T, Base_Traits, 1000000000000000000 >;
    template< typename T, class Base_Traits > using  peta_traits = mul_prefix_traits< T, Base_Traits, 1000000000000000    >;
    template< typename T, class Base_Traits > using  tera_traits = mul_prefix_traits< T, Base_Traits, 1000000000000       >;
    template< typename T, class Base_Traits > using  giga_traits = mul_prefix_traits< T, Base_Traits, 1000000000          >;
    template< typename T, class Base_Traits > using  mega_traits = mul_prefix_traits< T, Base_Traits, 1000000             >;
    template< typename T, class Base_Traits > using  kilo_traits = mul_prefix_traits< T, Base_Traits, 1000                >;
    template< typename T, class Base_Traits > using hecto_traits = mul_prefix_traits< T, Base_Traits, 100                 >;
    template< typename T, class Base_Traits > using  deca_traits = mul_prefix_traits< T, Base_Traits, 10                  >;
    
    template< typename T, class Base_Traits > using  deci_traits = div_prefix_traits< T, Base_Traits, 10                  >;
    template< typename T, class Base_Traits > using centi_traits = div_prefix_traits< T, Base_Traits, 100                 >;
    template< typename T, class Base_Traits > using milli_traits = div_prefix_traits< T, Base_Traits, 1000                >;
    template< typename T, class Base_Traits > using micro_traits = div_prefix_traits< T, Base_Traits, 1000000             >;
    template< typename T, class Base_Traits > using  nano_traits = div_prefix_traits< T, Base_Traits, 1000000000          >;
    template< typename T, class Base_Traits > using  pico_traits = div_prefix_traits< T, Base_Traits, 1000000000000       >;
    template< typename T, class Base_Traits > using femto_traits = div_prefix_traits< T, Base_Traits, 1000000000000000    >;
    template< typename T, class Base_Traits > using  atto_traits = div_prefix_traits< T, Base_Traits, 1000000000000000000 >;
    
    
    template< typename T, class Traits > using   exa = unit< T,   exa_traits< T, Traits > >;
    template< typename T, class Traits > using  peta = unit< T,  peta_traits< T, Traits > >;
    template< typename T, class Traits > using  tera = unit< T,  tera_traits< T, Traits > >;
    template< typename T, class Traits > using  giga = unit< T,  giga_traits< T, Traits > >;
    template< typename T, class Traits > using  mega = unit< T,  mega_traits< T, Traits > >;
    template< typename T, class Traits > using  kilo = unit< T,  kilo_traits< T, Traits > >;
    template< typename T, class Traits > using hecto = unit< T, hecto_traits< T, Traits > >;
    template< typename T, class Traits > using  deca = unit< T,  deca_traits< T, Traits > >;
    
    template< typename T, class Traits > using  deci = unit< T,  deci_traits< T, Traits > >;
    template< typename T, class Traits > using centi = unit< T, centi_traits< T, Traits > >;
    template< typename T, class Traits > using milli = unit< T, milli_traits< T, Traits > >;
    template< typename T, class Traits > using micro = unit< T, micro_traits< T, Traits > >;
    template< typename T, class Traits > using  nano = unit< T,  nano_traits< T, Traits > >;
    template< typename T, class Traits > using  pico = unit< T,  pico_traits< T, Traits > >;
    template< typename T, class Traits > using femto = unit< T, femto_traits< T, Traits > >;
    template< typename T, class Traits > using  atto = unit< T,  atto_traits< T, Traits > >;
} }


namespace JadeMatrix { namespace units // Unit string specializations //////////
{
    #define PREFIX_UNIT_STRING_SPECIALIZATION( FACTOR, FACTOR_OPERAND, OPERAND_CLASS, NAME_PREFIX, SYMBOL_PREFIX ) \
    template< \
        typename T, \
        class Base_Traits \
    > struct OPERAND_CLASS< T, Base_Traits, FACTOR > \
    { \
        using value_type = T; \
         \
        static const std::string& unit_name() \
        { \
            static const std::string s = NAME_PREFIX + std::string( \
                Base_Traits::unit_name() \
            ); \
            return s; \
        } \
        static const std::string& unit_symbol() \
        { \
            static const std::string s = SYMBOL_PREFIX + std::string( \
                Base_Traits::unit_symbol() \
            ); \
            return s; \
        } \
         \
        template< typename O, class Other_Traits > \
        static constexpr \
        T convert_from( const unit< O, Other_Traits >& from ) \
        { \
            return Base_Traits::convert_from( from ) FACTOR_OPERAND static_cast< T >( FACTOR ); \
        } \
    };
    
    PREFIX_UNIT_STRING_SPECIALIZATION( 1000000000000000000, /, mul_prefix_traits, "exa"  , "E"  );
    PREFIX_UNIT_STRING_SPECIALIZATION( 1000000000000000   , /, mul_prefix_traits, "peta" , "P"  );
    PREFIX_UNIT_STRING_SPECIALIZATION( 1000000000000      , /, mul_prefix_traits, "tera" , "T"  );
    PREFIX_UNIT_STRING_SPECIALIZATION( 1000000000         , /, mul_prefix_traits, "giga" , "G"  );
    PREFIX_UNIT_STRING_SPECIALIZATION( 1000000            , /, mul_prefix_traits, "mega" , "M"  );
    PREFIX_UNIT_STRING_SPECIALIZATION( 1000               , /, mul_prefix_traits, "kilo" , "k"  );
    PREFIX_UNIT_STRING_SPECIALIZATION( 100                , /, mul_prefix_traits, "hecto", "h"  );
    PREFIX_UNIT_STRING_SPECIALIZATION( 10                 , /, mul_prefix_traits, "deca" , "da" );
    
    PREFIX_UNIT_STRING_SPECIALIZATION( 10                 , *, div_prefix_traits, "deci" , "d"  );
    PREFIX_UNIT_STRING_SPECIALIZATION( 100                , *, div_prefix_traits, "centi", "c"  );
    PREFIX_UNIT_STRING_SPECIALIZATION( 1000               , *, div_prefix_traits, "milli", "m"  );
    PREFIX_UNIT_STRING_SPECIALIZATION( 1000000            , *, div_prefix_traits, "micro", "μ"  );
    PREFIX_UNIT_STRING_SPECIALIZATION( 1000000000         , *, div_prefix_traits, "nano" , "n"  );
    PREFIX_UNIT_STRING_SPECIALIZATION( 1000000000000      , *, div_prefix_traits, "pico" , "p"  );
    PREFIX_UNIT_STRING_SPECIALIZATION( 1000000000000000   , *, div_prefix_traits, "femto", "f"  );
    PREFIX_UNIT_STRING_SPECIALIZATION( 1000000000000000000, *, div_prefix_traits, "atto" , "a"  );
    
    // TODO: dozen, semi-, bi-, etc.
    
    #undef PREFIX_UNIT_STRING_SPECIALIZATION
} }


namespace JadeMatrix { namespace units // Helper for defining prefixed units ///
{
    #define ALL_PREFIXES_FOR_UNIT( SINGULAR, PLURAL ) \
    template< typename T > using   exa##PLURAL =   exa< T, SINGULAR##_traits< T > >; \
    template< typename T > using  peta##PLURAL =  peta< T, SINGULAR##_traits< T > >; \
    template< typename T > using  tera##PLURAL =  tera< T, SINGULAR##_traits< T > >; \
    template< typename T > using  giga##PLURAL =  giga< T, SINGULAR##_traits< T > >; \
    template< typename T > using  mega##PLURAL =  mega< T, SINGULAR##_traits< T > >; \
    template< typename T > using  kilo##PLURAL =  kilo< T, SINGULAR##_traits< T > >; \
    template< typename T > using hecto##PLURAL = hecto< T, SINGULAR##_traits< T > >; \
    template< typename T > using  deca##PLURAL =  deca< T, SINGULAR##_traits< T > >; \
     \
    template< typename T > using  deci##PLURAL =  deci< T, SINGULAR##_traits< T > >; \
    template< typename T > using centi##PLURAL = centi< T, SINGULAR##_traits< T > >; \
    template< typename T > using milli##PLURAL = milli< T, SINGULAR##_traits< T > >; \
    template< typename T > using micro##PLURAL = micro< T, SINGULAR##_traits< T > >; \
    template< typename T > using  nano##PLURAL =  nano< T, SINGULAR##_traits< T > >; \
    template< typename T > using  pico##PLURAL =  pico< T, SINGULAR##_traits< T > >; \
    template< typename T > using femto##PLURAL = femto< T, SINGULAR##_traits< T > >; \
    template< typename T > using  atto##PLURAL =  atto< T, SINGULAR##_traits< T > >;
} }


#endif
