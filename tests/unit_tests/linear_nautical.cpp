#include <main.hpp>
#include <units/linear.hpp>

namespace units = ::JadeMatrix::units;

void test()
{
    {
        units::miles< double > miles{ 7 };
        units::fathoms< double > fathoms{ miles };
        CHECK( static_cast< double >( fathoms ) == 6160.0 );
    }
    {
        units::centimeters< double > centimeters{ 7 };
        units::fathoms< double > fathoms{ centimeters };
        CHECK( static_cast< double >( fathoms ) == 7.0 / 182.88 );
    }
    {
        units::fathoms< double > fathoms{ 7 };
        units::miles< double > miles{ fathoms };
        CHECK( static_cast< double >( miles ) == 7.0 / 880.0 );
    }
    {
        units::fathoms< double > fathoms{ 7 };
        units::centimeters< double > centimeters{ fathoms };
        CHECK( static_cast< double >( centimeters ) == 1280.16 );
    }
    
    {
        units::nautical_miles< double > nautical_miles{ 7 };
        units::inches< double > inches{ nautical_miles };
        CHECK( static_cast< double >( inches ) == 1296400.0 / 2.54 );
    }
    {
        units::nautical_miles< double > nautical_miles{ 7 };
        units::feet< double > feet{ nautical_miles };
        CHECK( static_cast< double >( feet ) == 1296400.0 / 30.48 );
    }
    {
        units::nautical_miles< double > nautical_miles{ 7 };
        units::meters< double > meters{ nautical_miles };
        CHECK( static_cast< double >( meters ) == 12964.0 );
    }
    {
        units::meters< double > meters{ 7 };
        units::nautical_miles< double > nautical_miles{ meters };
        CHECK( static_cast< double >( nautical_miles ) == 7.0 / 1852.0 );
    }
}
