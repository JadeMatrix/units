#include <units/angular.hpp>
#include <units/linear.hpp>


namespace units = ::JadeMatrix::units;


int main( int, char*[] )
{
    {
        units::feet< double > f1{ 7 };
        units::feet< double > f2{ 3 };
        
        auto ff = f1 / f2;
        
        units::radians< double > r1{ 7 };
        units::radians< double > r2{ 3 };
        
        auto rr = r1 / r2;
        
        ff = rr;
    }
    {
        auto r1 = ( (
              units::    meters< double >{ 3 }
            / units::decimeters< double >{ 7 }
        ) / (
              units::kilometers< double >{  4 }
            / units::    meters< double >{ 20 }
        ) );
        units::ratio< double > r2 = r1;
        
        auto r3 = ( (
              units::nautical_miles< double >{ 3 }
            / units::degrees       < double >{ 7 }
        ) / (
              units::  kiloyards< double >{  4 }
            / units::revolutions< double >{ 20 }
        ) );
        units::ratio< double > r4 = r3;
        
        auto r5 = r1 / r3;
        units::ratio< double > r6 = r5;
        r6 = r2 / r4;
    }
    
    return 0;
}
