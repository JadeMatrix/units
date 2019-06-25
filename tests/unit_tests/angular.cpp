#include <main.hpp>
#include <units/internal/constants.hpp>
#include <units/angular.hpp>

namespace units = ::JadeMatrix::units;

void test()
{
    {
        units::radians< double > radians{ 7 };
        units::degrees< double > degrees{ radians };
        CHECK( static_cast< double >( degrees ) == (
            7.0 * 180.0 / units::constants< double >::pi
        ) );
    }
    {
        units::radians< double > radians{ 7 };
        units::arcminutes< double > arcminutes{ radians };
        CHECK( static_cast< double >( arcminutes ) == (
            7.0 * 10800.0 / units::constants< double >::pi
        ) );
    }
    {
        units::degrees< double > degrees{ 7 };
        units::arcminutes< double > arcminutes{ degrees };
        CHECK( static_cast< double >( arcminutes ) == 420.0 );
    }
    
    {
        units::degrees< double > degrees{ 7 };
        units::radians< double > radians{ degrees };
        CHECK( static_cast< double >( radians ) == (
            7.0 * units::constants< double >::pi / 180.0
        ) );
    }
    {
        units::arcminutes< double > arcminutes{ 7 };
        units::radians< double > radians{ arcminutes };
        CHECK( static_cast< double >( radians ) == (
            7.0 * units::constants< double >::pi / 10800.0
        ) );
    }
    {
        units::arcminutes< double > arcminutes{ 7 };
        units::degrees< double > degrees{ arcminutes };
        CHECK( static_cast< double >( degrees ) == 7.0 / 60.0 );
    }
    
    {
        units::arcminutes< double > arcminutes{ 7 };
        units::arcseconds< double > arcseconds{ arcminutes };
        CHECK( static_cast< double >( arcseconds ) == 420.0 );
    }
    {
        units::arcseconds< double > arcseconds{ 7 };
        units::arcminutes< double > arcminutes{ arcseconds };
        CHECK( static_cast< double >( arcminutes ) == 7.0 / 60.0 );
    }
    
    {
        units::revolutions< double > revolutions{ 7 };
        units::degrees< double > degrees{ revolutions };
        CHECK( static_cast< double >( degrees ) == 2520.0 );
    }
    {
        units::revolutions< double > revolutions{ 7 };
        units::radians< double > radians{ revolutions };
        CHECK( static_cast< double >( radians ) == (
            14.0 * units::constants< double >::pi
        ) );
    }
    {
        units::revolutions< double > revolutions{ 7 };
        units::arcminutes< double > arcminutes{ revolutions };
        CHECK( static_cast< double >( arcminutes ) == 151200.0 );
    }
    {
        units::revolutions< double > revolutions{ 7 };
        units::arcseconds< double > arcseconds{ revolutions };
        CHECK( static_cast< double >( arcseconds ) == 9072000.0 );
    }
}
