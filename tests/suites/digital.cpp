#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/digital.hpp>
#include <units/stringify/to_string.hpp>


namespace units = ::JadeMatrix::units;


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
