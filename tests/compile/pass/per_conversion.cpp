#include <units/angular.hpp>
#include <units/linear.hpp>
#include <units/temporal.hpp>


namespace units = ::JadeMatrix::units;


int main( int, char*[] )
{
    {
        units::per< units::feet, units::yards, double > fpy{ 29 };
        fpy = units::ratio< double >{ 4 };
    }
    {
        units::per<
            units::revolutions,
            units::minutes,
            double
        > rpm{ 29 };
        rpm = units::per<
            units::kiloradians,
            units::microseconds,
            double
        >{ 1234 };
    }
    {
        units::per<
            units::per<
                units::centirevolutions,
                units::gigafeet
            >::template unit_type,
            units::per<
                units::ratio,
                units::femtohours
            >::template unit_type,
            double
        > foo{ 893402 };
        units::per<
            units::per<
                units::hectoarcseconds,
                units::micro_nautical_miles
            >::template unit_type,
            units::per<
                units::per<
                    units::millifathoms,
                    units::nanoinches
                >::template unit_type,
                units::dozen_minutes
            >::template unit_type,
            double
        > bar { 2 };
        // Honestly no idea what the conversion ends up being but it's correct
        foo = bar;
    }
    
    return 0;
}
