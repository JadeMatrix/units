#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/linear.hpp>


namespace units = ::JadeMatrix::units;



TEST_CASE( "miles to fathoms" )
{
    units::miles< double > miles{ 7 };
    units::fathoms< double > fathoms{ miles };
    REQUIRE( static_cast< double >( fathoms ) == 6160.0 );
}

TEST_CASE( "centimeters to fathoms" )
{
    units::centimeters< double > centimeters{ 7 };
    units::fathoms< double > fathoms{ centimeters };
    REQUIRE( static_cast< double >( fathoms ) == 7.0 / 182.88 );
}

TEST_CASE( "fathoms to miles" )
{
    units::fathoms< double > fathoms{ 7 };
    units::miles< double > miles{ fathoms };
    REQUIRE( static_cast< double >( miles ) == 7.0 / 880.0 );
}

TEST_CASE( "fathoms to centimeters" )
{
    units::fathoms< double > fathoms{ 7 };
    units::centimeters< double > centimeters{ fathoms };
    REQUIRE( static_cast< double >( centimeters ) == 1280.16 );
}


TEST_CASE( "nautical miles to inches" )
{
    units::nautical_miles< double > nautical_miles{ 7 };
    units::inches< double > inches{ nautical_miles };
    REQUIRE( static_cast< double >( inches ) == 1296400.0 / 2.54 );
}

TEST_CASE( "nautical miles to feet" )
{
    units::nautical_miles< double > nautical_miles{ 7 };
    units::feet< double > feet{ nautical_miles };
    REQUIRE( static_cast< double >( feet ) == 1296400.0 / 30.48 );
}

TEST_CASE( "nautical miles to meters" )
{
    units::nautical_miles< double > nautical_miles{ 7 };
    units::meters< double > meters{ nautical_miles };
    REQUIRE( static_cast< double >( meters ) == 12964.0 );
}

TEST_CASE( "meters to nautical miles" )
{
    units::meters< double > meters{ 7 };
    units::nautical_miles< double > nautical_miles{ meters };
    REQUIRE( static_cast< double >( nautical_miles ) == 7.0 / 1852.0 );
}
