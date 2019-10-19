#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/core/constants.hpp>
#include <units/angular.hpp>
#include <units/stringify/to_string.hpp>


namespace units = ::JadeMatrix::units;


// Tests use `long double` values as these are only checking for numerically
// correct conversions (not type semantics preservation)

TEST_CASE( "radians to degrees" )
{
    units::radians< long double > radians{ 7 };
    units::degrees< long double > degrees{ radians };
    REQUIRE( static_cast< long double >( degrees ) == (
        7.0L * 180.0L / units::constants::pi< long double >::value
    ) );
}
TEST_CASE( "radians to arcminutes" )
{
    units::radians< long double > radians{ 7 };
    units::arcminutes< long double > arcminutes{ radians };
    REQUIRE( static_cast< long double >( arcminutes ) == (
        7.0L * 10800.0L / units::constants::pi< long double >::value
    ) );
}
TEST_CASE( "degrees to arcminutes" )
{
    units::degrees< long double > degrees{ 7 };
    units::arcminutes< long double > arcminutes{ degrees };
    REQUIRE( static_cast< long double >( arcminutes ) == 420.0L );
}

TEST_CASE( "degrees to radians" )
{
    units::degrees< long double > degrees{ 7 };
    units::radians< long double > radians{ degrees };
    REQUIRE( static_cast< long double >( radians ) == (
        7.0L * units::constants::pi< long double >::value / 180.0L
    ) );
}
TEST_CASE( "arcminutes to radians" )
{
    units::arcminutes< long double > arcminutes{ 7 };
    units::radians< long double > radians{ arcminutes };
    REQUIRE( static_cast< long double >( radians ) == (
        7.0L * units::constants::pi< long double >::value / 10800.0L
    ) );
}
TEST_CASE( "arcminutes to degrees" )
{
    units::arcminutes< long double > arcminutes{ 7 };
    units::degrees< long double > degrees{ arcminutes };
    REQUIRE( static_cast< long double >( degrees ) == 7.0L / 60.0L );
}

TEST_CASE( "arcminutes to arcseconds" )
{
    units::arcminutes< long double > arcminutes{ 7 };
    units::arcseconds< long double > arcseconds{ arcminutes };
    REQUIRE( static_cast< long double >( arcseconds ) == 420.0L );
}
TEST_CASE( "arcseconds to arcminutes" )
{
    units::arcseconds< long double > arcseconds{ 7 };
    units::arcminutes< long double > arcminutes{ arcseconds };
    REQUIRE( static_cast< long double >( arcminutes ) == 7.0L / 60.0L );
}

TEST_CASE( "revolutions to degrees" )
{
    units::revolutions< long double > revolutions{ 7 };
    units::degrees< long double > degrees{ revolutions };
    REQUIRE( static_cast< long double >( degrees ) == 2520.0L );
}
TEST_CASE( "revolutions to radians" )
{
    units::revolutions< long double > revolutions{ 7 };
    units::radians< long double > radians{ revolutions };
    REQUIRE( static_cast< long double >( radians ) == (
        14.0L * units::constants::pi< long double >::value
    ) );
}
TEST_CASE( "revolutions to arcminutes" )
{
    units::revolutions< long double > revolutions{ 7 };
    units::arcminutes< long double > arcminutes{ revolutions };
    REQUIRE( static_cast< long double >( arcminutes ) == 151200.0L );
}
TEST_CASE( "revolutions to arcseconds" )
{
    units::revolutions< long double > revolutions{ 7 };
    units::arcseconds< long double > arcseconds{ revolutions };
    REQUIRE( static_cast< long double >( arcseconds ) == 9072000.0L );
}

TEST_CASE( "rpm stringification specialization" )
{
    units::semirpm< int > srpm{ 5 };
    REQUIRE( to_string( srpm ) == "5rpm*2" );
}
