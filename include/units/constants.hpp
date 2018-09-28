#pragma once
#ifndef JM_UNITS_CONSTANTS_HPP
#define JM_UNITS_CONSTANTS_HPP


namespace JadeMatrix { namespace units
{
    template< typename T> struct constants
    {
        static constexpr T pi             = static_cast< T >( 3.1415926535897932385L );
        static constexpr T circle_degrees = static_cast< T >( 360L                   );
        static constexpr T hour_minutes   = static_cast< T >( 60L                    );
        static constexpr T inch_cms       = static_cast< T >( 2.54L                  );
        static constexpr T foot_inches    = static_cast< T >( 12L                    );
        static constexpr T meter_cms      = static_cast< T >( 100L                   );
    };
} }


#endif
