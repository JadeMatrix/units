#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <units/core/units.hpp>
#include <units/core/scales.hpp>
#include <units/core/strings.hpp>


namespace units = ::JadeMatrix::units;


namespace asdf
{
    struct foo_traits
    {
        template< typename From > struct convert_from;
    };
    
    struct bar
    {
        int value;
    };
    
    // template<> struct foo_traits::convert_from< bar >
    // {
    //     template< typename To > static constexpr To to( const bar& b )
    //     {
    //         return static_cast< To >( b.value );
    //     }
    // };
}

DEFINE_ALL_PREFIXES_FOR_UNIT( foos, asdf::foo_traits )
DEFINE_ALL_STRINGS_FOR_UNIT( foos, "foos", "¿" )


TEST_CASE( "custom unit" )
{
    // {
    //     units::radians< double > radians{ 7 };
    //     units::degrees< double > degrees{ radians };
    //     CHECK( static_cast< double >( degrees ) == (
    //         7.0 * 180.0 / units::constants< double >::pi
    //     ) );
    // }
    
    // asdf::bar b{ 1 };
    // /*asdf::*/foos< float > f{ /*b*/ };
    // /*asdf::*/megafoos< int > F{ f };
}
