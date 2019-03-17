#pragma once
#ifndef JM_UNITS_INTERNAL_PER_HPP
#define JM_UNITS_INTERNAL_PER_HPP


#include "core_types.hpp"
#include "traits_utils.hpp"


namespace JadeMatrix { namespace units // Unit divided by another //////////////
{
    template<
        template< typename > class Numer_Unit,
        template< typename > class Denom_Unit,
        typename T
    > class per
    {
    public:
        template< typename O > using numer_unit = Numer_Unit< O >;
        template< typename O > using denom_unit = Denom_Unit< O >;
        using value_type = T;
        template< typename O > using unit_type = per<
            Numer_Unit,
            Denom_Unit,
            O
        >;
        
    protected:
        value_type _value;
        
    public:
        constexpr per() {}
        constexpr per( const value_type& v ) : _value{ v } {}
        template<
            template< typename > class Other_Numer_Unit,
            template< typename > class Other_Denom_Unit,
            typename O
        > constexpr per( const per<
            Other_Numer_Unit,
            Other_Denom_Unit,
            O
        >& o ) : _value{
            static_cast< O >( numer_unit< O >{
                Other_Numer_Unit< O >{ static_cast< O >( o ) }
            } ) / static_cast< O >( denom_unit< O >{
                Other_Denom_Unit< O >{ static_cast< O >( 1 ) }
            } )
        }
        {}
        
        template<
            typename O = value_type,
            typename = typename std::enable_if< !is_unit< O >::value >::type
        > explicit constexpr operator O () const
        {
            return static_cast< O >( _value );
        }
    };
    
    template<
        template< typename > class Numer_Unit,
        template< typename > class Denom_Unit
    > class per< Numer_Unit, Denom_Unit, void >
    {
    public:
        template< typename O > using numer_unit = Numer_Unit< O >;
        template< typename O > using denom_unit = Denom_Unit< O >;
        using value_type = void;
        template< typename O > using unit_type = per<
            numer_unit,
            denom_unit,
            O
        >;
        
        per() = delete;
        ~per() = delete;
    };
} }


#endif
