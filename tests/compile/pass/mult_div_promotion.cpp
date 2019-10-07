#include <units/linear.hpp>

#include <type_traits>  // is_same


namespace units = ::JadeMatrix::units;


int main( int, char*[] )
{
    {
        auto d = units::feet< double >{ 3 } * 7;
        static_assert(
            std::is_same< decltype( d ), units::feet< double > >::value,
            "!std::is_same< decltype( d ), units::feet< double > >::value"
        );
    }
    {
        auto d = units::centimiles< short >{ 3 } * 7L;
        static_assert(
            std::is_same< decltype( d ), units::centimiles< long > >::value,
            "!std::is_same< decltype( d ), units::centimiles< long > >::value"
        );
    }
    {
        auto d = units::kilometers< char >{ 48 } / 6.0;
        static_assert(
            std::is_same< decltype( d ), units::kilometers< double > >::value,
            "!std::is_same< decltype( d ), units::kilometers< double > >::value"
        );
    }
    
    return 0;
}
