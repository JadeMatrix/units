#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/linear.hpp>
#include <units/stringify/to_string.hpp>


namespace units = ::JadeMatrix::units;


// Tests use `long double` values as these are only checking for numerically
// correct conversions (not type semantics preservation)

TEST_CASE( "miles to fathoms" )
{
    units::miles< long double > miles{ 7 };
    units::fathoms< long double > fathoms{ miles };
    REQUIRE( static_cast< long double >( fathoms ) == 6160.0L );
}
TEST_CASE( "centimeters to fathoms" )
{
    units::centimeters< long double > centimeters{ 7 };
    units::fathoms< long double > fathoms{ centimeters };
    REQUIRE( static_cast< long double >( fathoms ) == 7.0L / 182.88L );
}
TEST_CASE( "fathoms to miles" )
{
    units::fathoms< long double > fathoms{ 7 };
    units::miles< long double > miles{ fathoms };
    REQUIRE( static_cast< long double >( miles ) == 7.0L / 880.0L );
}
TEST_CASE( "fathoms to centimeters" )
{
    units::fathoms< long double > fathoms{ 7 };
    units::centimeters< long double > centimeters{ fathoms };
    REQUIRE( static_cast< long double >( centimeters ) == 1280.16L );
}

TEST_CASE( "nautical miles to inches" )
{
    units::nautical_miles< long double > nautical_miles{ 7 };
    units::inches< long double > inches{ nautical_miles };
    REQUIRE( static_cast< long double >( inches ) == 1296400.0L / 2.54L );
}
TEST_CASE( "nautical miles to feet" )
{
    units::nautical_miles< long double > nautical_miles{ 7 };
    units::feet< long double > feet{ nautical_miles };
    REQUIRE( static_cast< long double >( feet ) == 1296400.0L / 30.48L );
}
TEST_CASE( "nautical miles to meters" )
{
    units::nautical_miles< long double > nautical_miles{ 7 };
    units::meters< long double > meters{ nautical_miles };
    REQUIRE( static_cast< long double >( meters ) == 12964.0L );
}
TEST_CASE( "meters to nautical miles" )
{
    units::meters< long double > meters{ 7 };
    units::nautical_miles< long double > nautical_miles{ meters };
    REQUIRE( static_cast< long double >( nautical_miles ) == 7.0L / 1852.0L );
}

TEST_CASE( "knots stringification specialization" )
{
    units::kiloknots< int > kkt{ 30 };
    REQUIRE( to_string( kkt ) == "30kkt" );
}
