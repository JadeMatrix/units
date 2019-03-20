#include <main.hpp>
#include <units/internal/constants.hpp>
#include <units/temporal.hpp>
#include <units/stream_format.hpp>

#include <sstream>

namespace units = ::JadeMatrix::units;

void test()
{
    {
        std::stringstream s;
        s << units::seconds< int >{ 7 };
        CHECK( s.str() == "7s" );
    }
    {
        std::chrono::seconds std_seconds{ 7 };
        units::seconds< double > seconds{ std_seconds };
        CHECK( static_cast< double >( seconds ) == 7.0 );
    }
    {
        std::chrono::minutes std_minutes{ 7 };
        units::seconds< double > seconds{ std_minutes };
        CHECK( static_cast< double >( seconds ) == 420.0 );
    }
    {
        units::seconds< long long > seconds{ 7 };
        std::chrono::seconds std_seconds{ seconds };
        CHECK( std_seconds.count() == 7 );
    }
    
    {
        units::seconds< double > seconds{ 7 };
        units::minutes< double > minutes{ seconds };
        CHECK( static_cast< double >( minutes ) == ( 7.0 / 60.0 ) );
    }
    {
        units::seconds< double > seconds{ 7 };
        units::hours< double > hours{ seconds };
        CHECK( static_cast< double >( hours ) == ( 7.0 / 3600.0 ) );
    }
    {
        units::minutes< double > minutes{ 7 };
        units::hours< double > hours{ minutes };
        CHECK( static_cast< double >( hours ) == ( 7.0 / 60.0 ) );
    }
    
    {
        units::minutes< double > minutes{ 7 };
        units::seconds< double > seconds{ minutes };
        CHECK( static_cast< double >( seconds ) == 420.0 );
    }
    {
        units::hours< double > hours{ 7 };
        units::seconds< double > seconds{ hours };
        CHECK( static_cast< double >( seconds ) == 25200.0 );
    }
    {
        units::hours< double > hours{ 7 };
        units::minutes< double > minutes{ hours };
        CHECK( static_cast< double >( minutes ) == 420.0 );
    }
}
