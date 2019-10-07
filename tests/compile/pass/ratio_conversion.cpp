#include <units/angular.hpp>
#include <units/linear.hpp>

#include <type_traits>  // is_same


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
    {
        auto v = units::miles< int >( 7 ) * units::ratio< int >( 1 );
        static_assert(
            std::is_same< decltype( v ), units::miles< int > >::value,
            "!std::is_same< decltype( v ), units::miles< int > >::value"
        );
    }
    {
        auto v = units::ratio< int >( 1 ) * units::meters< int >( 7 );
        static_assert(
            std::is_same< decltype( v ), units::meters< int > >::value,
            "!std::is_same< decltype( v ), units::meters< int > >::value"
        );
    }
    {
        auto v = units::inches< int >( 7 ) / units::ratio< int >( 1 );
        static_assert(
            std::is_same< decltype( v ), units::inches< int > >::value,
            "!std::is_same< decltype( v ), units::inches< int > >::value"
        );
    }
    {
        auto v = units::ratio< int >( 1 ) / units::feet< int >( 7 );
        static_assert(
            units::internal::is_same_unit<
                decltype( v )::template unit_type,
                units::per<
                    units::ratio,
                    units::feet,
                    void
                >::template unit_type
            >::value,
            "!std::is_same< decltype( v ), units::per< units::ratio, "
            "units::feet, int > >::value"
        );
    }
    
    return 0;
}
