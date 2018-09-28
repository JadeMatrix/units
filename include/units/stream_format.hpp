#pragma once
#ifndef JM_UNITS_STREAM_FORMAT_HPP
#define JM_UNITS_STREAM_FORMAT_HPP


#include "unit.hpp"
#include "strings.hpp"

#include <ostream>


namespace JadeMatrix { namespace units // Stream formatting operators //////////
{
    template< typename Traits, typename T, typename Scale >
    std::ostream& operator<<(
        std::ostream& out,
        const unit< Traits, T, Scale >& u
    )
    {
        return out
            << static_cast< T >( u )
            << unit_strings< unit< Traits, T, Scale > >::symbol()
        ;
    }
    
    template<
        template< typename > typename Numer_Unit,
        template< typename > typename Denom_Unit,
        typename T
    >
    std::ostream& operator<<(
        std::ostream& out,
        const per< Numer_Unit, Denom_Unit, T >& u
    )
    {
        return out
            << static_cast< T >( u )
            << unit_strings< per< Numer_Unit, Denom_Unit, T > >::symbol()
        ;
    }
    
    template<
        template< typename > typename First_Unit,
        template< typename > typename Second_Unit,
        typename T
    >
    std::ostream& operator<<(
        std::ostream& out,
        const by< First_Unit, Second_Unit, T >& u
    )
    {
        return out
            << static_cast< T >( u )
            << unit_strings< by< First_Unit, Second_Unit, T > >::symbol()
        ;
    }
    
    template< typename T >
    std::ostream& operator<<( std::ostream& out, const ratio< T >& u )
    {
        return out << static_cast< T >( u );
    }
} }


#endif