#include <units/linear.hpp>
#include <units/temporal.hpp>


namespace units = ::JadeMatrix::units;


int main( int, char*[] )
{
    {
        units::feet< float  > f1{ 7 };
        units::feet< double > f2{ 3 };
        f2 = f1;
    }
    {
        units::feet< int  > f1{ 7 };
        units::feet< long > f2{ 3 };
        f2 = f1;
    }
    {
        units::feet< unsigned short > f1{ 7 };
        units::feet< unsigned int   > f2{ 3 };
        f2 = f1;
    }
    {
        units::feet< int   > f1{ 7 };
        units::feet< float > f2{ 3 };
        f2 = f1;
    }
    
    {
        units::seconds< float  > s1{ 7 };
        units::seconds< double > s2{ 3 };
        s2 = s1;
    }
    {
        units::seconds< int  > s1{ 7 };
        units::seconds< long > s2{ 3 };
        s2 = s1;
    }
    {
        units::seconds< unsigned short > s1{ 7 };
        units::seconds< unsigned int   > s2{ 3 };
        s2 = s1;
    }
    {
        units::seconds< int   > s1{ 7 };
        units::seconds< float > s2{ 3 };
        s2 = s1;
    }
    
    return 0;
}
