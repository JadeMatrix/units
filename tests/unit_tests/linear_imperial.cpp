#include <main.hpp>
#include <units/linear.hpp>

namespace units = ::JadeMatrix::units;

void test()
{
    {
        units::feet< double > feet{ 7 };
        units::inches< double > inches{ feet };
        CHECK( static_cast< double >( inches ) == 84.0 );
    }
    {
        units::yards< double > yards{ 7 };
        units::feet< double > feet{ yards };
        CHECK( static_cast< double >( feet ) == 21.0 );
    }
    {
        units::yards< double > yards{ 7 };
        units::inches< double > inches{ yards };
        CHECK( static_cast< double >( inches ) == 252.0 );
    }
    {
        units::miles< double > miles{ 7 };
        units::feet< double > feet{ miles };
        CHECK( static_cast< double >( feet ) == 36960.0 );
    }
    
    {
        units::inches< double > inches{ 7 };
        units::feet< double > feet{ inches };
        CHECK( static_cast< double >( feet ) == 7.0 / 12.0 );
    }
    {
        units::feet< double > feet{ 7 };
        units::yards< double > yards{ feet };
        CHECK( static_cast< double >( yards ) == 7.0 / 3.0 );
    }
    {
        units::inches< double > inches{ 7 };
        units::yards< double > yards{ inches };
        CHECK( static_cast< double >( yards ) == 7.0 / 36.0 );
    }
    {
        units::feet< double > feet{ 7 };
        units::miles< double > miles{ feet };
        CHECK( static_cast< double >( miles ) == 7.0 / 5280.0 );
    }
}
