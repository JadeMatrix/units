#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/core/internal/core_type_detection.hpp>
#include <units/digital.hpp>
#include <units/stringify/to_string.hpp>
#include <units/temporal.hpp>


namespace units = ::JadeMatrix::units;


TEST_CASE( "floatops / seconds = FLOPS" )
{
    auto v1 = (
        units::floatops< long double >{ 3.0L }
        / units::seconds< int >{ 1 }
    );
    auto v2 = units::flops< long double >{ 3.0L };
    
    REQUIRE( v1 == v2 );
    REQUIRE( units::internal::is_same_unit<
        decltype( v1 )::template unit_type,
        decltype( v2 )::template unit_type
    >::value );
}

TEST_CASE( "instructions / seconds = IPS" )
{
    auto v1 = (
        units::instructions< long double >{ 3.0L }
        / units::seconds< int >{ 1 }
    );
    auto v2 = units::ips< long double >{ 3.0L };
    
    REQUIRE( v1 == v2 );
    REQUIRE( units::internal::is_same_unit<
        decltype( v1 )::template unit_type,
        decltype( v2 )::template unit_type
    >::value );
}


TEST_CASE( "FLOPS stringification specialization" )
{
    units::milliflops< int > mFLOPS{ 937 };
    REQUIRE( to_string( mFLOPS ) == "937mFLOPS" );
}

TEST_CASE( "IPS stringification specialization" )
{
    units::gibiips< int > GiIPS{ 0 };
    REQUIRE( to_string( GiIPS ) == "0GiIPS" );
}
