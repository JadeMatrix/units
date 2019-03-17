#include <main.hpp>
#include <units/linear.hpp>

namespace units = ::JadeMatrix::units;

void test()
{
    units::inches< double > inches{ 1 };
    units::centimeters< double > centimeters{ inches };
    CHECK( static_cast< double >( centimeters ) == 2.54 );
}
