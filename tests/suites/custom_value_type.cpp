#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "custom_value_type.hpp"

#include <units/angular.hpp>
#include <units/linear.hpp>
#include <units/temporal.hpp>

#include <type_traits>  // is_same


namespace units = ::JadeMatrix::units;


TEST_CASE( "tagged works" )
{
    struct tag0 {};
    struct tag1 {};
    struct tag2 {};
    
    ::custom::tagged< float, tag0 > t0{ 00.00f };
    ::custom::tagged< float, tag1 > t1{ 11.11f };
    ::custom::tagged< float, tag2 > t2{ 22.22f };
    
    auto t_t = t0 / t1;
    REQUIRE( std::is_same<
        decltype( t_t ),
        ::custom::tagged< float, tag0 > >::value
    );
    
    auto tt = t1 / t2;
    REQUIRE( std::is_same<
        decltype( tt ),
        ::custom::tagged< float, tag1 > >::value
    );
    
    auto tnt = t2 == t0;
    REQUIRE( std::is_same<
        decltype( tnt ),
        ::custom::tagged< bool, tag2 > >::value
    );
}

TEST_CASE( "custom value type works" )
{
    ::custom::bar nan_bar{ ::custom::nan };
    REQUIRE( to_string( nan_bar ) == "非" );
    
    ::custom::bar inf_bar{ ::custom::inf };
    REQUIRE( to_string( inf_bar ) == "正無" );
    
    ::custom::bar num_bar{ 1234.567L };
    // REQUIRE( to_string( num_bar ) == "一二三四点五六七" );
    
    REQUIRE( std::is_same<
        decltype( nan_bar == num_bar ),
        ::custom::tagged< ::custom::bar::cmp, ::custom::bar::eq_tag >
    >::value );
    REQUIRE( (
        ( nan_bar == num_bar ) == ::custom::bar::cmp::invalid
    ).value() );
    
    REQUIRE( std::is_same<
        decltype( nan_bar != num_bar ),
        ::custom::tagged< ::custom::bar::cmp, ::custom::bar::ne_tag >
    >::value );
    REQUIRE( (
        ( nan_bar != num_bar ) == ::custom::bar::cmp::invalid
    ).value() );
    
    nan_bar = num_bar;
    
    REQUIRE( ( ( nan_bar == num_bar ) == ::custom::bar::cmp::yes ).value() );
}

#define DEFINE_ARITHMETIC_PASSTHROUGH_TEST_CASE( OPERAND, TAGNAME, LHS, RHS ) \
TEST_CASE( "custom value type operator " #OPERAND " passthrough" ) \
{ \
    units::LHS< ::custom::bar > val1{ 123.4L }; \
    units::RHS< ::custom::bar > val2{ 123.4L }; \
     \
    REQUIRE( std::is_same< \
        decltype( val1 OPERAND val2 )::value_type, \
        ::custom::tagged< ::custom::bar, ::custom::bar::TAGNAME##_tag > \
    >::value ); \
}

DEFINE_ARITHMETIC_PASSTHROUGH_TEST_CASE( *, mul, revolutions,      radians )
DEFINE_ARITHMETIC_PASSTHROUGH_TEST_CASE( /, div, nanoseconds,        hours )
DEFINE_ARITHMETIC_PASSTHROUGH_TEST_CASE( %, mod, dozen_hours, microminutes )
DEFINE_ARITHMETIC_PASSTHROUGH_TEST_CASE( +, plu,      meters,    kiloyards )
DEFINE_ARITHMETIC_PASSTHROUGH_TEST_CASE( -, min,  arcseconds,   arcminutes )

#undef DEFINE_ARITHMETIC_PASSTHROUGH_TEST_CASE

#define DEFINE_COMPARE_PASSTHROUGH_TEST_CASE( OPERAND, TAGNAME, LHS, RHS ) \
TEST_CASE( "custom value type operator " #OPERAND " passthrough" ) \
{ \
    units::LHS< ::custom::bar > val1{ 123.4L }; \
    units::RHS< ::custom::bar > val2{ 123.4L }; \
     \
    REQUIRE( std::is_same< \
        decltype( val1 OPERAND val2 ), \
        ::custom::tagged< ::custom::bar::cmp, ::custom::bar::TAGNAME##_tag > \
    >::value ); \
}

DEFINE_COMPARE_PASSTHROUGH_TEST_CASE( ==, eq,           gigameters,   nanometers )
DEFINE_COMPARE_PASSTHROUGH_TEST_CASE( !=, ne,          revolutions,  revolutions )
DEFINE_COMPARE_PASSTHROUGH_TEST_CASE( > , gt,              minutes,      seconds )
DEFINE_COMPARE_PASSTHROUGH_TEST_CASE( < , lt,                 feet,       inches )
DEFINE_COMPARE_PASSTHROUGH_TEST_CASE( >=, ge,       nanoarcseconds, centiradians )
DEFINE_COMPARE_PASSTHROUGH_TEST_CASE( <=, le, dozen_nautical_miles,  centimeters )

#undef DEFINE_COMPARE_PASSTHROUGH_TEST_CASE

#define DEFINE_ASSIGN_NO_PASSTHROUGH_TEST_CASE( OPERAND, LHS, RHS ) \
TEST_CASE( "custom value type operator " #OPERAND "= no passthrough" ) \
{ \
    units::LHS< ::custom::bar > val1{ 123.4L }; \
    units::RHS< ::custom::bar > val2{ 123.4L }; \
     \
    REQUIRE( std::is_same< \
        decltype( val1 OPERAND##= val2 ), \
        units::LHS< ::custom::bar >& \
    >::value ); \
}

DEFINE_ASSIGN_NO_PASSTHROUGH_TEST_CASE( *, revolutions,      radians )
DEFINE_ASSIGN_NO_PASSTHROUGH_TEST_CASE( /, nanoseconds,        hours )
DEFINE_ASSIGN_NO_PASSTHROUGH_TEST_CASE( %, dozen_hours, microminutes )
DEFINE_ASSIGN_NO_PASSTHROUGH_TEST_CASE( +,      meters,    kiloyards )
DEFINE_ASSIGN_NO_PASSTHROUGH_TEST_CASE( -,  arcseconds,   arcminutes )

#undef DEFINE_ASSIGN_NO_PASSTHROUGH_TEST_CASE
