#pragma once
#ifndef UNITS_ANGULAR_HPP
#define UNITS_ANGULAR_HPP


#include "unit.hpp"
#include "constants.hpp"

#include <string>


namespace units // Declarations ////////////////////////////////////////////////
{
    template< typename T > struct    degree_traits;
    template< typename T > struct    radian_traits;
    template< typename T > struct arcminute_traits;
    
    template< typename T > using    degrees = unit< T,    degree_traits< T > >;
    template< typename T > using    radians = unit< T,    radian_traits< T > >;
    template< typename T > using arcminutes = unit< T, arcminute_traits< T > >;
}


namespace units // Degrees /////////////////////////////////////////////////////
{
    template< typename T > struct degree_traits
    {
        using value_type = T;
        
        static std::string unit_string()
        {
            return "°";
        }
        
        template< typename O >
        static constexpr
        T convert_from( const degrees< O >& from )
        {
            return ( O )from;
        }
        
        // Conversions from other units ////////////////////////////////////////
        
        template< typename O >
        static constexpr
        T convert_from( const radians< O >& from )
        {
            return (
                ( O )from
                * ( ( T )constants::circle_degrees / ( T )2 )
                / constants::pi
            );
        }
        
        template< typename O >
        static constexpr
        T convert_from( const arcminutes< O >& from )
        {
            return ( O )from / ( T )constants::hour_minutes;
        }
    };
}


namespace units // Radians /////////////////////////////////////////////////////
{
    template< typename T > struct radian_traits
    {
        using value_type = T;
        
        static std::string unit_string()
        {
            return "r";
        }
        
        template< typename O >
        static constexpr
        T convert_from( const radians< O >& from )
        {
            return ( O )from;
        }
        
        // Conversions from other units ////////////////////////////////////////
        
        template< typename O >
        static constexpr
        T convert_from( const degrees< O >& from )
        {
            return (
                ( T )from
                * constants::pi
                / ( ( T )constants::circle_degrees / ( T )2 )
            );
        }
        
        template< typename O >
        static constexpr
        T convert_from( const arcminutes< O >& from )
        {
            return ( O )radians< O >( degrees< O >( from ) );
        }
    };
}


namespace units // Arcminutes //////////////////////////////////////////////////
{
    template< typename T > struct arcminute_traits
    {
        using value_type = T;
        
        static std::string unit_string()
        {
            return "moa";
        }
        
        template< typename O >
        static constexpr
        T convert_from( const arcminutes< O >& from )
        {
            return ( O )from;
        }
        
        // Conversions from other units ////////////////////////////////////////
        
        template< typename O >
        static constexpr
        T convert_from( const degrees< O >& from )
        {
            return ( O )from * ( T )constants::hour_minutes;
        }
        
        template< typename O >
        static constexpr
        T convert_from( const radians< O >& from )
        {
            return ( O )degrees< O >( from ) * ( T )constants::hour_minutes;
        }
    };
}


#endif
