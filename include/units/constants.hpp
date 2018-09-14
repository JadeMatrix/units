#pragma once
#ifndef JM_UNITS_CONSTANTS_HPP
#define JM_UNITS_CONSTANTS_HPP


namespace JadeMatrix { namespace units
{
    template< typename T> struct constants
    {
        static constexpr T pi             = static_cast< T >( 3.1415926535897932385L );
        static constexpr T circle_degrees = static_cast< T >( 360                    );
        static constexpr T hour_minutes   = static_cast< T >( 60L                    );
    };
} }


#endif
