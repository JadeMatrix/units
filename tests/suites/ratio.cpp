#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/angular.hpp>
#include <units/linear.hpp>
#include <units/temporal.hpp>
#include <units/stringify/ostream.hpp>


namespace units = ::JadeMatrix::units;


TEST_CASE( "preserve relative scales in ratio (larger numerator)" )
{
    auto r = (
          units::kilometers< double >{ 1 }
        / units::    meters< double >{ 1 }
    );
    REQUIRE( r == units::ratio< double >( 1000.0 ) );
}

TEST_CASE( "preserve relative scales in ratio (larger denominator)" )
{
    auto r = (
          units::centihours< double >{ 1 }
        / units::     hours< double >{ 1 }
    );
    REQUIRE( r == units::ratio< double >( 0.01 ) );
}

TEST_CASE(
    "preserve relative scales in ratio (reduce to 1, convertible units)"
)
{
    auto r1 = (
          units::     degrees< double >{ 1 }
        / units::millidegrees< double >{ 1 }
    );
    auto r2 = (
          units::kilorevolutions< double >{ 1 }
        / units::    revolutions< double >{ 1 }
    );
    REQUIRE( ( r1 / r2 ) == units::ratio< double >( 1.0 ) );
}

TEST_CASE(
    "preserve relative scales in ratio (reduce to 1, non-convertible units)"
)
{
    auto r1 = (
          units::     radians< double >{ 1 }
        / units::microradians< double >{ 1 }
    );
    auto r2 = (
          units::megafeet< double >{ 1 }
        / units::    feet< double >{ 1 }
    );
    REQUIRE( ( r1 / r2 ) == units::ratio< double >( 1.0 ) );
}
