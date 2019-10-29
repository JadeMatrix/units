#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/core/internal/core_type_detection.hpp>
#include <units/stringify/ostream.hpp>
#include <units/temporal.hpp>

#include <sstream>


namespace units = ::JadeMatrix::units;


// Tests use `long double` values as these are only checking for numerically
// correct conversions (not type semantics preservation)

TEST_CASE( "stream format seconds" )
{
    std::stringstream s;
    s << units::seconds< int >{ 7 };
    REQUIRE( s.str() == "7s" );
}

TEST_CASE( "std::chrono::seconds to JadeMatrix::units::seconds" )
{
    std::chrono::seconds std_seconds{ 7 };
    using rep = std::chrono::seconds::rep;
    units::seconds< rep > seconds{ std_seconds };
    REQUIRE( static_cast< rep >( seconds ) == 7 );
}
TEST_CASE( "std::chrono::minutes to JadeMatrix::units::seconds" )
{
    std::chrono::minutes std_minutes{ 7 };
    using rep = std::chrono::minutes::rep;
    units::seconds< rep > seconds{ std_minutes };
    REQUIRE( static_cast< rep >( seconds ) == 420 );
}
TEST_CASE( "JadeMatrix::units::seconds to std::chrono::seconds" )
{
    units::seconds< long long > seconds{ 7 };
    std::chrono::seconds std_seconds{ seconds };
    REQUIRE( std_seconds.count() == 7 );
}
TEST_CASE( "JadeMatrix::units::minutes to std::chrono::seconds" )
{
    units::minutes< long long > minutes{ 7 };
    std::chrono::seconds std_seconds{ minutes };
    REQUIRE( std_seconds.count() == 420 );
}

TEST_CASE( "seconds to minutes" )
{
    units::seconds< long double > seconds{ 7 };
    units::minutes< long double > minutes{ seconds };
    REQUIRE( static_cast< long double >( minutes ) == ( 7.0L / 60.0L ) );
}
TEST_CASE( "seconds to hours" )
{
    units::seconds< long double > seconds{ 7 };
    units::hours< long double > hours{ seconds };
    REQUIRE( static_cast< long double >( hours ) == ( 7.0L / 3600.0L ) );
}
TEST_CASE( "minutes to hours" )
{
    units::minutes< long double > minutes{ 7 };
    units::hours< long double > hours{ minutes };
    REQUIRE( static_cast< long double >( hours ) == ( 7.0L / 60.0L ) );
}

TEST_CASE( "minutes to seconds" )
{
    units::minutes< long double > minutes{ 7 };
    units::seconds< long double > seconds{ minutes };
    REQUIRE( static_cast< long double >( seconds ) == 420.0L );
}
TEST_CASE( "hours to seconds" )
{
    units::hours< long double > hours{ 7 };
    units::seconds< long double > seconds{ hours };
    REQUIRE( static_cast< long double >( seconds ) == 25200.0L );
}
TEST_CASE( "hours to minutes" )
{
    units::hours< long double > hours{ 7 };
    units::minutes< long double > minutes{ hours };
    REQUIRE( static_cast< long double >( minutes ) == 420.0L );
}

TEST_CASE( "ratio / seconds = hertz" )
{
    auto v1 = (
        units::ratio< long double >{ 40000.0L }
        / units::seconds< int >{ 1 }
    );
    auto v2 = units::hertz< long double >{ 40000.0L };
    
    REQUIRE( v1 == v2 );
    REQUIRE( units::internal::is_same_unit<
        decltype( v1 )::template unit_type,
        decltype( v2 )::template unit_type
    >::value );
}
