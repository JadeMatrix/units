#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/linear.hpp>


namespace units = ::JadeMatrix::units;


// Tests use `long double` values as these are only checking for numerically
// correct conversions (not type semantics preservation)

TEST_CASE( "feet to inches" )
{
    units::feet< long double > feet{ 7 };
    units::inches< long double > inches{ feet };
    REQUIRE( static_cast< long double >( inches ) == 84.0L );
}
TEST_CASE( "yards to feet" )
{
    units::yards< long double > yards{ 7 };
    units::feet< long double > feet{ yards };
    REQUIRE( static_cast< long double >( feet ) == 21.0L );
}
TEST_CASE( "yards to inches" )
{
    units::yards< long double > yards{ 7 };
    units::inches< long double > inches{ yards };
    REQUIRE( static_cast< long double >( inches ) == 252.0L );
}
TEST_CASE( "miles to feet" )
{
    units::miles< long double > miles{ 7 };
    units::feet< long double > feet{ miles };
    REQUIRE( static_cast< long double >( feet ) == 36960.0L );
}

TEST_CASE( "inches to feet" )
{
    units::inches< long double > inches{ 7 };
    units::feet< long double > feet{ inches };
    REQUIRE( static_cast< long double >( feet ) == 7.0L / 12.0L );
}
TEST_CASE( "feet to yards" )
{
    units::feet< long double > feet{ 7 };
    units::yards< long double > yards{ feet };
    REQUIRE( static_cast< long double >( yards ) == 7.0L / 3.0L );
}
TEST_CASE( "inches to yards" )
{
    units::inches< long double > inches{ 7 };
    units::yards< long double > yards{ inches };
    REQUIRE( static_cast< long double >( yards ) == 7.0L / 36.0L );
}
TEST_CASE( "feet to miles" )
{
    units::feet< long double > feet{ 7 };
    units::miles< long double > miles{ feet };
    REQUIRE( static_cast< long double >( miles ) == 7.0L / 5280.0L );
}
