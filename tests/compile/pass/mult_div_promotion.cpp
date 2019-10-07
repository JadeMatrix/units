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
    {
        auto d = 48.0 / units::kilometers< char >{ 6 };
        static_assert(
            units::internal::is_same_unit<
                decltype( d )::template unit_type,
                units::per<
                    units::ratio,
                    units::kilometers,
                    void
                >::template unit_type
            >::value && std::is_same<
                decltype( d )::value_type,
                double
            >::value,
            "!std::is_same< decltype( d ), units::per< units::ratio, "
            "units::kilometers, double > >::value"
        );
    }
    
    return 0;
}
