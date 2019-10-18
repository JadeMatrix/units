# Units for C++11

This project aims to provide a type-safe, efficient, and easy-to-use C++11 library for representing real-world units.  It is similar to a more abstract version of `std::duration<>` available in the [C++ `<chrono>` header](https://en.cppreference.com/w/cpp/chrono).

The requirements of this project are:

* No common or preferred system of measure (e.g. storing everything as metric and converting as needed)
* Respect the user's types
    * No common, intermediate storage type (e.g. storing all numbers as `double`)
    * All operations on units should preserve value types as if just the value types were used
* Zero-cost abstraction
    * Do nothing at runtime that can't be done at compile-time
    * All unit conversions must result in the same value that would result from hand-coding a multiplication by a one-way conversion factor

## Available Types

Any real-world unit of measure that has a concrete definition or unambiguous meaning is welcome in this library (e.g. [pennyweight](https://en.wikipedia.org/wiki/Pennyweight) or [bytes](https://en.wikipedia.org/wiki/Byte), but not [cubits](https://en.wikipedia.org/wiki/Cubit)).  If one you know of is missing, please [create an issue](https://github.com/JadeMatrix/units/issues/new) or make a pull request.  In case your project needs a unit that wouldn't be appropriate for inclusion here, see [*Creating Custom Units*](#creating-custom-units).

Currently, a few linear, angular, and temporal units are provided in their own headers; a full set of standard units are planned by v1.0.  The available units are:

* Angular — `units/angular.hpp`
    * `radians`
    * `degrees`
    * `arcminutes`
    * `arcseconds`
    * `revolutions`
    * `rpm`
* Linear — `units/linear.hpp`
    * `inches`
    * `feet`
    * `yards`
    * `miles`
    * `meters`
    * `fathoms`
    * `nautical_miles` (Note: prefixed versions follow the pattern `kilo_nautical_miles`, `dozen_nautical_miles`, etc.)
    * `knots`
* Temporal — `units/temporal.hpp` — these are also implicitly convertible to & from [`std::chrono::duration<>`s](https://en.cppreference.com/w/cpp/chrono/duration)
    * `seconds`
    * `minutes`
    * `hours`
* Digital — `units/digital.hpp`
    * bits
    * bytes
    * [flops](https://en.wikipedia.org/wiki/FLOPS)
    * [ips](https://en.wikipedia.org/wiki/Instructions_per_second)

All unit types have scaled versions for the following prefixes:

* All the [SI prefixes](https://en.wikipedia.org/wiki/Metric_prefix)
    * Everything from `atto*` to `exa*`
    * `zepto*`/`zetta*` and `yocto*`/`yotta*` [if the platform `std::intmax_t` can represent them](https://en.cppreference.com/w/cpp/numeric/ratio/ratio)
* All the [ISO/IEC 80000 binary prefixes](https://en.wikipedia.org/wiki/Binary_prefix)
    * Everything from `kibi*` to `exbi*`
    * `zebi*` and `yobi*`, again only if the platform `std::intmax_t` can represent them
* `dozen_*`
* `bi*`
* `semi*`

## Installation & Integration

While primarily header-only, `units` uses modern (v3.0+) [CMake](https://cmake.org/) to build & run its unit tests, as well as export configuration for an `INTERFACE`-only library.  While CMake will attempt installation at the system level by default, that is not recommended as it may interfere with the system's package manager(s).  To install and use `units` from an arbitrary directory, run something similar to following:

```sh
git clone https://github.com/JadeMatrix/units.git $UNIT_SOURCE
cd $UNITS_BUILD
cmake -D CMAKE_INSTALL_PREFIX=$UNITS_INSTALL $UNIT_SOURCE
make all test install
```

… where `$UNIT_SOURCE`, `$UNITS_BUILD`, and `$UNITS_INSTALL` are the places where `units` is cloned, built, and installed to, respectively.  While these can all be the same location, this is not recommended as build and install outputs may interfere with (overwrite) the cloned source code.

Then, to use `units` in your own CMake project, add the `lib/cmake` under your `units` install location to your project's [`CMAKE_PREFIX_PATH`](https://cmake.org/cmake/help/latest/variable/CMAKE_PREFIX_PATH.html) variable:

```sh
cd /your/build/dir/
cmake -D CMAKE_PREFIX_PATH=$UNITS_INSTALL/lib/cmake/ /your/project/source/
```

Then, in your project's `CMakeLists.txt`:

```cmake
FIND_PACKAGE( JadeMatrix-units 0.2 REQUIRED COMPONENTS units )
```

There will now be an imported target by the name `JadeMatrix::units::units` which you can pass to [`TARGET_LINK_LIBRARIES()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html).

## Usage

`units` follows the `<author>::<library>::` namespacing convention, which is intended to be aliased to simply `units::` in most cases but permits disambiguation if required.

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

`units` also has an optional headers providing stringification functionality, all of which use [argument-dependent lookup (ADL)](https://en.cppreference.com/w/cpp/language/adl) for value types:

| Stringify method | Header |
|---|---|
| [`to_string()`](https://en.cppreference.com/w/cpp/string/basic_string/to_string) | `units/stringify/to_string.hpp` |
| [Stream format operators](https://en.cppreference.com/w/cpp/named_req/FormattedOutputFunction) (`<<`) | `units/stringify/ostream.hpp` |

```cpp
#include <units/linear.hpp>
#include <units/stringify/ostream.hpp>
#include <iostream>

namespace units = ::JadeMatrix::units;

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

## Creating Custom Units

Creating custom units is generally very straightforward, essentially consisting of a traits/tag type and a number of relationships representing unit conversions.  Some utility macros are also provided to define prefixed versions and add stringification support to new units.  For example, the units `inches` and `feet` are defined basically as:

```cpp
#include <units/core/constants.hpp>
// This header contains includes of all the necessary utilities for defining
// custom units (except constants)
#include <units/core/define_unit.hpp>

namespace custom
{
    struct inch_traits {};
    struct foot_traits {};

    // Conversion relationship /////////////////////////////////////////////////

    struct inches_feet_linear_relation
    {
        template< typename T > struct values
        {
            // Default `slope_num` of 1 used if not defined
            static constexpr auto slope_num = 
                units::constants::foot_inches< T >::value;
            // Default `slope_den` of 1 used if not defined
            // Default `intercept` of 0 used if not defined
        };
    };
    inches_feet_linear_relation units_linear_relation_lookup(
        inch_traits&&,
        foot_traits&&
    ); // No definition for this function; used only for ADL logic

    // Prefixes definition /////////////////////////////////////////////////////

    #define DEFINE_PREFIX_FOR_inches( PREFIX, SCALE ) \
        template< typename T > using PREFIX##inches = units::unit< \
            inch_traits, \
            SCALE, \
            T \
        >;
    #define DEFINE_PREFIX_FOR_feet( PREFIX, SCALE ) \
        template< typename T > using PREFIX##feet = units::unit< \
            foot_traits, \
            SCALE, \
            T \
        >;
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_inches )
    JM_UNITS_FOREACH_SCALE( DEFINE_PREFIX_FOR_feet   )
    #undef DEFINE_PREFIX_FOR_inches
    #undef DEFINE_PREFIX_FOR_feet

    // Stringification support for all prefixes ////////////////////////////////

    DEFINE_ALL_STRINGS_FOR_UNIT( inch_traits, "inches", "in" )
    DEFINE_ALL_STRINGS_FOR_UNIT( foot_traits,   "feet", "ft" )
}

// Test that the units were defined correctly
#include <type_traits>
static_assert(
    std::is_convertible<
        custom::megafeet< float >,
        custom::centiinches< float >
    >::value
    && std::is_convertible<
        custom::dozen_inches< float >,
        custom::bifeet< float >
    >::value,
    "feet & inches not mutually convertible"
);
```

In this case a partial specialization of `traits_linear_relation< A, B, T >` represents how much of type A there is in type B — that is,

```
A = B * slope_num / slope_den + intercept
```

Note that only one relationship partial overload is necessary; the library will swap the numerator and denominator as needed depending on the direction of the conversion.
