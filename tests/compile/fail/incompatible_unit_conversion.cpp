#include <units/angular.hpp>
#include <units/linear.hpp>


namespace units = ::JadeMatrix::units;


int main( int, char*[] )
{
    units::feet< double > feet{ 3 };
    units::radians< double > radians{ feet };
    return 0;
}
