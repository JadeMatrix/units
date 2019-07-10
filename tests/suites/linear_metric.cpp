#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/linear.hpp>


namespace units = ::JadeMatrix::units;


// Tests use `long double` values as these are only checking for numerically
// correct conversions (not type semantics preservation)

TEST_CASE( "meters to centimeters" )
{
    units::meters< long double > meters{ 7 };
    units::centimeters< long double > centimeters{ meters };
    REQUIRE( static_cast< long double >( centimeters ) == 700.0L );
}
TEST_CASE( "kilometers to meters" )
{
    units::kilometers< long double > kilometers{ 7 };
    units::meters< long double > meters{ kilometers };
    REQUIRE( static_cast< long double >( meters ) == 7000.0L );
}
TEST_CASE( "kilometers to centimeters" )
{
    units::kilometers< long double > kilometers{ 7 };
    units::centimeters< long double > centimeters{ kilometers };
    REQUIRE( static_cast< long double >( centimeters ) == 700000.0L );
}

TEST_CASE( "centimeters to meters" )
{
    units::centimeters< long double > centimeters{ 7 };
    units::meters< long double > meters{ centimeters };
    REQUIRE( static_cast< long double >( meters ) == 0.07L );
}
TEST_CASE( "meters to kilometers" )
{
    units::meters< long double > meters{ 7 };
    units::kilometers< long double > kilometers{ meters };
    REQUIRE( static_cast< long double >( kilometers ) == 0.007L );
}
TEST_CASE( "centimeters to kilometers" )
{
    units::centimeters< long double > centimeters{ 7 };
    units::kilometers< long double > kilometers{ centimeters };
    REQUIRE( static_cast< long double >( kilometers ) == 0.00007L );
}
