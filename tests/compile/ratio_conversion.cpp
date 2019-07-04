#include <units/angular.hpp>
#include <units/linear.hpp>


namespace units = ::JadeMatrix::units;


int main( int, char*[] )
{
    units::feet< double > f1{ 7 };
    units::feet< double > f2{ 3 };
    
    auto ff = f1 / f2;
    
    units::radians< double > r1{ 7 };
    units::radians< double > r2{ 3 };
    
    auto rr = r1 / r2;
    
    ff = rr;
    
    return 0;
}
