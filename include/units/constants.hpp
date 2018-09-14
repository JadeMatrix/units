#pragma once
#ifndef YAVSG_MATH_CONSTANTS_HPP
#define YAVSG_MATH_CONSTANTS_HPP


namespace yavsg
{
    namespace constants
    {
        template< typename T > constexpr T pi             = static_cast< T >( 3.1415926535897932385L );
        template< typename T > constexpr T circle_degrees = static_cast< T >( 360                    );
        template< typename T > constexpr T hour_minutes   = static_cast< T >( 60L                    );
    }
}


#endif
