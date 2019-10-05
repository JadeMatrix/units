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
        ::custom::tagged< float, tag0 >
    >::value );
    
    auto tt = t1 / t2;
    REQUIRE( std::is_same<
        decltype( tt ),
        ::custom::tagged< float, tag1 >
    >::value );
    
    auto tnt = t2 == t0;
    REQUIRE( std::is_same<
        decltype( tnt ),
        ::custom::tagged< bool, tag2 >
    >::value );
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
