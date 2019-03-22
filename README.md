# Units for C++11

This project aims to provide a type-safe, efficient, and easy-to-use C++11 library for representing real-world units.  It is similar to a more abstract version of `std::duration<>` available in the [C++ `<chrono>` header](https://en.cppreference.com/w/cpp/chrono).


## Installation & Integration

While primarily header-only, `units` uses modern [CMake](https://cmake.org/) (3.0+) to build & run its unit tests, as well as export configuration for an `INTERFACE`-only library.  While CMake will attempt installation at the system level by default, that is not recommended as it may interfere with the system's package manager(s).  To install to & use `units` from an arbitrary directory, run something similar to following:

```sh
git clone https://github.com/JadeMatrix/units.git $SOURCE_DIRECTORY
cd $BUILD_DIRECTORY
cmake -D CMAKE_INSTALL_PREFIX=$INSTALL_DIRECTORY $SOURCE_DIRECTORY
make all test install
```

… where `$SOURCE_DIRECTORY`, `$BUILD_DIRECTORY`, and `$INSTALL_DIRECTORY` are the places where `units` is cloned, built, and installed to, respectively.  While these can all be the same location, this is not recommended as build and install outputs may interfere with (overwrite) the cloned source code.

Then, to use `units` in your own CMake project, add the `lib/cmake` under your `units` install location to your project's [`CMAKE_PREFIX_PATH`](https://cmake.org/cmake/help/latest/variable/CMAKE_PREFIX_PATH.html) variable.  Then, in your project's `CMakeLists.txt`:

```cmake
FIND_PACKAGE( JadeMatrix::units 0.2 REQUIRED COMPONENTS units )
```

There will now be an imported target by the name `JadeMatrix::units::units` which you can pass to [`TARGET_LINK_LIBRARIES()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html).

## Usage

`units` follows the `<author>::<library>::` namespacing convention, which is intended to be aliased to simply `units::` in most cases but be unambiguous if required.  Currently, a few linear and angular units are provided in their own headers; a full set of standard units are planned by v1.0.

```cpp
#include <units/angular.hpp>
#include <cmath>  // For std::sin()

namespace units = ::JadeMatrix::units;

// "Base" type-safe sine function taking radians as standard
template< typename T >
constexpr units::ratio< T > sin( const units::radians< T >& r )
{
    using std::sin;
    return sin( static_cast< T >( r ) );
}

// Wrapper sine function taking degrees
template< typename T >
constexpr units::ratio< T > sin( const units::degrees< T >& r )
{
    return sin( units::radians< T >{ r } );
}
```

`units` also has an optional header providing stream format operators for unit types:

```cpp
#include <units/linear.hpp>
#include <units/stream_format.hpp>
#include <iostream>

void print_kiloyards_in_feet( units::kiloyards< int > kyd )
{
    std::cout
        << kyd
        << " = "
        << units::feet< double >{ kyd }
        << std::endl
    ;
}
```

## Available Types

* Angular — `units/angular.hpp`
    * `radians`
    * `degrees`
    * `arcminutes`
* Linear — `units/linear.hpp`
    * `inches`
    * `feet`
    * `yards`
    * `miles`
    * `meters`
* Temporal — `units/temporal.hpp` — these are also implicitly convertible to & from [`std::chrono::duration<>`s](https://en.cppreference.com/w/cpp/chrono/duration)
    * `seconds`
    * `minutes`
    * `hours`

All unit types have versions for all the [SI prefixes](https://en.wikipedia.org/wiki/Metric_prefix) from `atto` to `exa`, `zepto`/`zetta` and `yocto`/`yotta` [if the platform `std::intmax_t` can represent them](https://en.cppreference.com/w/cpp/numeric/ratio/ratio), as well as `dozen_`, `bi`, and `semi`.

## Creating Custom Units

Creating custom units is generally very simple, consisting of a traits/tag type, two macros to define prefixed `unit` aliases and strings, and a number of relationships representing unit conversions.  For example, the units `inches` and `feet` are defined basically as:

```cpp
#include <units/units.hpp>
#include <units/scales.hpp>
#include <units/strings.hpp>

struct inch_traits {};
struct foot_traits {};

DEFINE_ALL_PREFIXES_FOR_UNIT( inches, inch_traits )
DEFINE_ALL_PREFIXES_FOR_UNIT(   feet, foot_traits )

DEFINE_ALL_STRINGS_FOR_UNIT( inches, "inches", "in" )
DEFINE_ALL_STRINGS_FOR_UNIT(   feet,   "feet", "ft" )

template< typename T >
struct ::JadeMatrix::units::traits_linear_relation<
    inch_traits,
    foot_traits,
    T
>
{
    static constexpr T slope_num = constants< T >::foot_inches;
    static constexpr T slope_den = static_cast< T >( 1 );
    static constexpr T intercept = static_cast< T >( 0 );
};
```

In this case a partial specialization of `traits_linear_relation< A, B, T >` represents how much of type A there is in type B — that is,

```
A = B * slope_num / slope_den + intercept
```

Note that only one relationship partial overload is necessary; the library will swap the numerator and denominator as needed depending on the direction of the conversion.
