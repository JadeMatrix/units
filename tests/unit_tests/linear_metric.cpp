#include <main.hpp>
#include <units/linear.hpp>

namespace units = ::JadeMatrix::units;

void test()
{
    {
        units::meters< double > meters{ 7 };
        units::centimeters< double > centimeters{ meters };
        CHECK( static_cast< double >( centimeters ) == 700.0 );
    }
    {
        units::kilometers< double > kilometers{ 7 };
        units::meters< double > meters{ kilometers };
        CHECK( static_cast< double >( meters ) == 7000.0 );
    }
    {
        units::kilometers< double > kilometers{ 7 };
        units::centimeters< double > centimeters{ kilometers };
        CHECK( static_cast< double >( centimeters ) == 700000.0 );
    }
    
    {
        units::centimeters< double > centimeters{ 7 };
        units::meters< double > meters{ centimeters };
        CHECK( static_cast< double >( meters ) == 0.07 );
    }
    {
        units::meters< double > meters{ 7 };
        units::kilometers< double > kilometers{ meters };
        CHECK( static_cast< double >( kilometers ) == 0.007 );
    }
    {
        units::centimeters< double > centimeters{ 7 };
        units::kilometers< double > kilometers{ centimeters };
        CHECK( static_cast< double >( kilometers ) == 0.00007 );
    }
}
