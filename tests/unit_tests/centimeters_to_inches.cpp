#include <main.hpp>
#include <units/linear.hpp>

namespace units = ::JadeMatrix::units;

void test()
{
    units::centimeters< double > centimeters{ 1 };
    units::inches< double > inches{ centimeters };
    CHECK( static_cast< double >( inches ) == ( 1.0 / 2.54 ) );
}
