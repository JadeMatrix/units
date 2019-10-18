#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/digital.hpp>
#include <units/core/internal/core_type_detection.hpp>


namespace units = ::JadeMatrix::units;

namespace
{
    struct foo {};
}


TEST_CASE( "`is_basic_unit` true for `unit`" )
{
    REQUIRE( units::internal::is_basic_unit< units::kibibytes< int > >::value );
}

TEST_CASE( "`is_basic_unit` false for `per`" )
{
    REQUIRE( !units::internal::is_basic_unit< units::per<
        units::gigabits,
        units::teraflops,
        double
    > >::value );
}

TEST_CASE( "`is_basic_unit` false for `by`" )
{
    REQUIRE( !units::internal::is_basic_unit< units::by<
        units::megaips,
        units::bytes,
        unsigned long
    > >::value );
}

TEST_CASE( "`is_basic_unit` false for `ratio`" )
{
    REQUIRE( !units::internal::is_basic_unit< units::ratio< float > >::value );
}

TEST_CASE( "`is_basic_unit` false for other types" )
{
    REQUIRE( !units::internal::is_basic_unit< foo >::value );
}


TEST_CASE( "`is_per` false for `unit`" )
{
    REQUIRE( !units::internal::is_per< units::semibits< double > >::value );
}

TEST_CASE( "`is_per` true for `per`" )
{
    REQUIRE( units::internal::is_per< units::decaflops< char > >::value );
}

TEST_CASE( "`is_per` false for `by`" )
{
    REQUIRE( !units::internal::is_per< units::by<
        units::dozen_bits,
        units::dozen_bytes,
        short
    > >::value );
}

TEST_CASE( "`is_per` false for `ratio`" )
{
    REQUIRE( !units::internal::is_per< units::ratio< unsigned int > >::value );
}

TEST_CASE( "`is_per` false for other types" )
{
    REQUIRE( !units::internal::is_per< foo >::value );
}


TEST_CASE( "`is_by` false for `unit`" )
{
    REQUIRE( !units::internal::is_by< units::decibits< long > >::value );
}

TEST_CASE( "`is_by` false for `per`" )
{
    REQUIRE( !units::internal::is_by< units::per<
        units::dozen_ips,
        units::exbiflops,
        unsigned char
    > >::value );
}

TEST_CASE( "`is_by` true for `by`" )
{
    REQUIRE( units::internal::is_by< units::by<
        units::flops,
        units::nanoips,
        unsigned short
    > >::value );
}

TEST_CASE( "`is_by` false for `ratio`" )
{
    REQUIRE( !units::internal::is_by< units::ratio< double > >::value );
}

TEST_CASE( "`is_by` false for other types" )
{
    REQUIRE( !units::internal::is_by< foo >::value );
}


TEST_CASE( "`is_ratio` false for `unit`" )
{
    REQUIRE( !units::internal::is_ratio< units::hectoips< char > >::value );
}

TEST_CASE( "`is_ratio` false for `per`" )
{
    REQUIRE( !units::internal::is_ratio< units::per<
        units::femtoips,
        units::nanobytes,
        long long int
    > >::value );
}

TEST_CASE( "`is_ratio` false for `by`" )
{
    REQUIRE( !units::internal::is_ratio< units::by<
        units::attobits,
        units::exaips,
        float
    > >::value );
}

TEST_CASE( "`is_ratio` true for `ratio`" )
{
    REQUIRE( units::internal::is_ratio< units::ratio< char > >::value );
}

TEST_CASE( "`is_ratio` false for other types" )
{
    REQUIRE( !units::internal::is_ratio< foo >::value );
}


TEST_CASE( "`is_unit` true for `unit`" )
{
    REQUIRE( units::internal::is_unit< units::microips< float > >::value );
}

TEST_CASE( "`is_unit` true for `per`" )
{
    REQUIRE( units::internal::is_unit< units::per<
        units::milliips,
        units::decabytes,
        float
    > >::value );
}

TEST_CASE( "`is_unit` true for `by`" )
{
    REQUIRE( units::internal::is_unit< units::by<
        units::tebibytes,
        units::flops,
        unsigned int
    > >::value );
}

TEST_CASE( "`is_unit` true for `ratio`" )
{
    REQUIRE( units::internal::is_unit< units::ratio< long int > >::value );
}

TEST_CASE( "`is_unit` false for other types" )
{
    REQUIRE( !units::internal::is_unit< foo >::value );
}


TEST_CASE( "`is_same_unit` true for two `ratio`s" )
{
    REQUIRE( units::internal::is_same_unit<
        units::ratio,
        units::ratio
    >::value );
}

TEST_CASE( "`is_same_unit` true for two same `unit`s" )
{
    REQUIRE( units::internal::is_same_unit<
        units::pebibits,
        units::pebibits
    >::value );
}

TEST_CASE( "`is_same_unit` true for two same `per`s" )
{
    using flops_per_ips = units::per<
        units::flops,
        units::ips,
        char
    >;
    REQUIRE( units::internal::is_same_unit<
        flops_per_ips::template unit_type,
        flops_per_ips::template unit_type
    >::value );
}

TEST_CASE( "`is_same_unit` true for two same `by`s" )
{
    using bits_bytes = units::by<
        units::bits,
        units::bytes,
        long double
    >;
    REQUIRE( units::internal::is_same_unit<
        bits_bytes::template unit_type,
        bits_bytes::template unit_type
    >::value );
}


TEST_CASE( "`is_same_unit` false for two different `unit`s" )
{
    REQUIRE( !units::internal::is_same_unit<
        units::bibits,
        units::bits
    >::value );
}

TEST_CASE( "`is_same_unit` false for two different `per`s" )
{
    using flops_per_ips = units::per<
        units::flops,
        units::ips,
        float
    >;
    using ips_per_flops = units::per<
        units::ips,
        units::flops,
        float
    >;
    REQUIRE( !units::internal::is_same_unit<
        flops_per_ips::template unit_type,
        ips_per_flops::template unit_type
    >::value );
}

TEST_CASE( "`is_same_unit` false for two different `by`s" )
{
    using bits_bytes = units::by<
        units::bits,
        units::bytes,
        short
    >;
    using bytes_bits = units::by<
        units::bytes,
        units::bits,
        short
    >;
    REQUIRE( !units::internal::is_same_unit<
        bits_bytes::template unit_type,
        bytes_bits::template unit_type
    >::value );
}

TEST_CASE( "`is_same_unit` false for mixed units" )
{
    REQUIRE( !units::internal::is_same_unit<
        units::ratio,
        units::picoflops
    >::value );
}
