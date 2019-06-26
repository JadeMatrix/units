#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/linear.hpp>


namespace units = ::JadeMatrix::units;


TEST_CASE( "feet to inches" )
{
    units::feet< double > feet{ 7 };
    units::inches< double > inches{ feet };
    REQUIRE( static_cast< double >( inches ) == 84.0 );
}

TEST_CASE( "yards to feet" )
{
    units::yards< double > yards{ 7 };
    units::feet< double > feet{ yards };
    REQUIRE( static_cast< double >( feet ) == 21.0 );
}

TEST_CASE( "yards to inches" )
{
    units::yards< double > yards{ 7 };
    units::inches< double > inches{ yards };
    REQUIRE( static_cast< double >( inches ) == 252.0 );
}

TEST_CASE( "miles to feet" )
{
    units::miles< double > miles{ 7 };
    units::feet< double > feet{ miles };
    REQUIRE( static_cast< double >( feet ) == 36960.0 );
}


TEST_CASE( "inches to feet" )
{
    units::inches< double > inches{ 7 };
    units::feet< double > feet{ inches };
    REQUIRE( static_cast< double >( feet ) == 7.0 / 12.0 );
}

TEST_CASE( "feet to yards" )
{
    units::feet< double > feet{ 7 };
    units::yards< double > yards{ feet };
    REQUIRE( static_cast< double >( yards ) == 7.0 / 3.0 );
}

TEST_CASE( "inches to yards" )
{
    units::inches< double > inches{ 7 };
    units::yards< double > yards{ inches };
    REQUIRE( static_cast< double >( yards ) == 7.0 / 36.0 );
}

TEST_CASE( "feet to miles" )
{
    units::feet< double > feet{ 7 };
    units::miles< double > miles{ feet };
    REQUIRE( static_cast< double >( miles ) == 7.0 / 5280.0 );
}
