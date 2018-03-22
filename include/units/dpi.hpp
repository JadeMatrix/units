#pragma once
#ifndef YAVSG_UNITS_DPI_HPP
#define YAVSG_UNITS_DPI_HPP


#include "unit.hpp"
#include "prefixes.hpp"

#include <string>


namespace yavsg { namespace dpi // Declarations ////////////////////////////////
{
    template< typename T > struct pixel_traits;
    template< typename T > struct point_traits;
    
    template< typename T > using pixels = unit< T, pixel_traits< T > >;
    template< typename T > using points = unit< T, point_traits< T > >;
} }


namespace yavsg { namespace dpi
{
    template< typename T > struct pixel_traits
    {
        using value_type = T;
        
        static const std::string& unit_name()
        {
            static const std::string s{ "pixel" };
            return s;
        }
        static const std::string& unit_symbol()
        {
            static const std::string s{ "px" };
            return s;
        }
        
        template< typename O >
        static constexpr
        T convert_from( const pixels< O >& from )
        {
            return static_cast< O >( from );
        }
    };
    
    template< typename T > struct point_traits
    {
        using value_type = T;
        
        static const std::string& unit_name()
        {
            static const std::string s{ "point" };
            return s;
        }
        static const std::string& unit_symbol()
        {
            static const std::string s{ "pt" };
            return s;
        }
        
        template< typename O >
        static constexpr
        T convert_from( const points< O >& from )
        {
            return static_cast< O >( from );
        }
    };
} }


namespace yavsg { namespace dpi // Prefixed versions of DPI units //////////////
{
    ALL_PREFIXES_FOR_UNIT( pixel, pixels )
    ALL_PREFIXES_FOR_UNIT( point, points )
} }


#endif
