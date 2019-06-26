#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/internal/constants.hpp>
#include <units/temporal.hpp>
#include <units/stream_format.hpp>

#include <sstream>


namespace units = ::JadeMatrix::units;



TEST_CASE( "stream format seconds" )
{
    std::stringstream s;
    s << units::seconds< int >{ 7 };
    REQUIRE( s.str() == "7s" );
}

TEST_CASE( "std::chrono::seconds to JadeMatrix::units::seconds" )
{
    std::chrono::seconds std_seconds{ 7 };
    units::seconds< double > seconds{ std_seconds };
    REQUIRE( static_cast< double >( seconds ) == 7.0 );
}

TEST_CASE( "std::chrono::minutes to JadeMatrix::units::seconds" )
{
    std::chrono::minutes std_minutes{ 7 };
    units::seconds< double > seconds{ std_minutes };
    REQUIRE( static_cast< double >( seconds ) == 420.0 );
}

TEST_CASE( "JadeMatrix::units::seconds to std::chrono::seconds" )
{
    units::seconds< long long > seconds{ 7 };
    std::chrono::seconds std_seconds{ seconds };
    REQUIRE( std_seconds.count() == 7 );
}


TEST_CASE( "seconds to minutes" )
{
    units::seconds< double > seconds{ 7 };
    units::minutes< double > minutes{ seconds };
    REQUIRE( static_cast< double >( minutes ) == ( 7.0 / 60.0 ) );
}

TEST_CASE( "seconds to hours" )
{
    units::seconds< double > seconds{ 7 };
    units::hours< double > hours{ seconds };
    REQUIRE( static_cast< double >( hours ) == ( 7.0 / 3600.0 ) );
}

TEST_CASE( "minutes to hours" )
{
    units::minutes< double > minutes{ 7 };
    units::hours< double > hours{ minutes };
    REQUIRE( static_cast< double >( hours ) == ( 7.0 / 60.0 ) );
}


TEST_CASE( "minutes to seconds" )
{
    units::minutes< double > minutes{ 7 };
    units::seconds< double > seconds{ minutes };
    REQUIRE( static_cast< double >( seconds ) == 420.0 );
}

TEST_CASE( "hours to seconds" )
{
    units::hours< double > hours{ 7 };
    units::seconds< double > seconds{ hours };
    REQUIRE( static_cast< double >( seconds ) == 25200.0 );
}

TEST_CASE( "hours to minutes" )
{
    units::hours< double > hours{ 7 };
    units::minutes< double > minutes{ hours };
    REQUIRE( static_cast< double >( minutes ) == 420.0 );
}
