#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/linear.hpp>


namespace units = ::JadeMatrix::units;


TEST_CASE( "meters to centimeters" )
{
    units::meters< double > meters{ 7 };
    units::centimeters< double > centimeters{ meters };
    REQUIRE( static_cast< double >( centimeters ) == 700.0 );
}

TEST_CASE( "kilometers to meters" )
{
    units::kilometers< double > kilometers{ 7 };
    units::meters< double > meters{ kilometers };
    REQUIRE( static_cast< double >( meters ) == 7000.0 );
}

TEST_CASE( "kilometers to centimeters" )
{
    units::kilometers< double > kilometers{ 7 };
    units::centimeters< double > centimeters{ kilometers };
    REQUIRE( static_cast< double >( centimeters ) == 700000.0 );
}


TEST_CASE( "centimeters to meters" )
{
    units::centimeters< double > centimeters{ 7 };
    units::meters< double > meters{ centimeters };
    REQUIRE( static_cast< double >( meters ) == 0.07 );
}

TEST_CASE( "meters to kilometers" )
{
    units::meters< double > meters{ 7 };
    units::kilometers< double > kilometers{ meters };
    REQUIRE( static_cast< double >( kilometers ) == 0.007 );
}

TEST_CASE( "centimeters to kilometers" )
{
    units::centimeters< double > centimeters{ 7 };
    units::kilometers< double > kilometers{ centimeters };
    REQUIRE( static_cast< double >( kilometers ) == 0.00007 );
}
