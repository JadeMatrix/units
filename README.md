# Units for C++11


## Usage

```cpp
namespace units = JadeMatrix::units;
void print_kiloyards_in_feet( units::kiloyards< double > kyd )
{
    std::cout
        << kyd
        << " = "
        << units::feet< int >{ kyd }
        << std::endl
    ;
}
```

```cpp
namespace units = JadeMatrix::units;
template< typename T > units::ratio< T > sin( const units::radians< T >& r )
{
    using std::sin;
    return sin( static_cast< T >( r ) );
}
template< typename T > units::ratio< T > sin( const units::degrees< T >& r )
{
    return sin( units::radians< T >{ r } );
}
```
