#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/core/constants.hpp>
#include <units/angular.hpp>


namespace units = ::JadeMatrix::units;



TEST_CASE( "radians to degrees" )
{
    units::radians< double > radians{ 7 };
    units::degrees< double > degrees{ radians };
    REQUIRE( static_cast< double >( degrees ) == (
        7.0 * 180.0 / units::constants< double >::pi
    ) );
}

TEST_CASE( "radians to arcminutes" )
{
    units::radians< double > radians{ 7 };
    units::arcminutes< double > arcminutes{ radians };
    REQUIRE( static_cast< double >( arcminutes ) == (
        7.0 * 10800.0 / units::constants< double >::pi
    ) );
}

TEST_CASE( "degrees to arcminutes" )
{
    units::degrees< double > degrees{ 7 };
    units::arcminutes< double > arcminutes{ degrees };
    REQUIRE( static_cast< double >( arcminutes ) == 420.0 );
}


TEST_CASE( "degrees to radians" )
{
    units::degrees< double > degrees{ 7 };
    units::radians< double > radians{ degrees };
    REQUIRE( static_cast< double >( radians ) == (
        7.0 * units::constants< double >::pi / 180.0
    ) );
}

TEST_CASE( "arcminutes to radians" )
{
    units::arcminutes< double > arcminutes{ 7 };
    units::radians< double > radians{ arcminutes };
    REQUIRE( static_cast< double >( radians ) == (
        7.0 * units::constants< double >::pi / 10800.0
    ) );
}

TEST_CASE( "arcminutes to degrees" )
{
    units::arcminutes< double > arcminutes{ 7 };
    units::degrees< double > degrees{ arcminutes };
    REQUIRE( static_cast< double >( degrees ) == 7.0 / 60.0 );
}


TEST_CASE( "arcminutes to arcseconds" )
{
    units::arcminutes< double > arcminutes{ 7 };
    units::arcseconds< double > arcseconds{ arcminutes };
    REQUIRE( static_cast< double >( arcseconds ) == 420.0 );
}

TEST_CASE( "arcseconds to arcminutes" )
{
    units::arcseconds< double > arcseconds{ 7 };
    units::arcminutes< double > arcminutes{ arcseconds };
    REQUIRE( static_cast< double >( arcminutes ) == 7.0 / 60.0 );
}


TEST_CASE( "revolutions to degrees" )
{
    units::revolutions< double > revolutions{ 7 };
    units::degrees< double > degrees{ revolutions };
    REQUIRE( static_cast< double >( degrees ) == 2520.0 );
}

TEST_CASE( "revolutions to radians" )
{
    units::revolutions< double > revolutions{ 7 };
    units::radians< double > radians{ revolutions };
    REQUIRE( static_cast< double >( radians ) == (
        14.0 * units::constants< double >::pi
    ) );
}

TEST_CASE( "revolutions to arcminutes" )
{
    units::revolutions< double > revolutions{ 7 };
    units::arcminutes< double > arcminutes{ revolutions };
    REQUIRE( static_cast< double >( arcminutes ) == 151200.0 );
}

TEST_CASE( "revolutions to arcseconds" )
{
    units::revolutions< double > revolutions{ 7 };
    units::arcseconds< double > arcseconds{ revolutions };
    REQUIRE( static_cast< double >( arcseconds ) == 9072000.0 );
}
