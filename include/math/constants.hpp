#pragma once
#ifndef YAVSG_MATH_CONSTANTS_HPP
#define YAVSG_MATH_CONSTANTS_HPP


namespace yavsg
{
    namespace constants
    {
        template< typename T > constexpr T pi             = ( T )3.1415926535897932385L;
        template< typename T > constexpr T circle_degrees = ( T )360;
        template< typename T > constexpr T hour_minutes   = ( T )60L;
    }
}


#endif
