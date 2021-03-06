#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "custom_unit.hpp"
#include "custom_value_type.hpp"

#include <units/stringify/to_string.hpp>


namespace units = ::JadeMatrix::units;


TEST_CASE( "custom unit symbol string" )
{
    REQUIRE( to_string( ::custom::foos< int >{ 3219 } ) == "3219¿" );
}

TEST_CASE( "custom unit to custom unit" )
{
    ::custom::foos< int > fi{ 423 };
    ::custom::foos< double > fd{ fi };
    REQUIRE( static_cast< double >( fd ) == 423.0f );
}
