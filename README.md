# Units for C++11

This project aims to provide a type-safe, efficient, and easy-to-use C++11 library for representing real-world units.  It is essentially a more abstract version of `std::duration<>` available in the [C++ `<chrono>` header](https://en.cppreference.com/w/cpp/chrono).


## Installation & Integration

While primarily header-only, `units` uses modern [CMake](https://cmake.org/) (3.0+) to build & run its unit tests, as well as export configuration for an `INTERFACE`-only library.  While CMake will attempt installation at the system level by default, that is not recommended as it may interfere with the system's package manager(s).  To install to & use `units` from an arbitrary directory, run something similar to following:

```sh
git clone https://github.com/JadeMatrix/units.git $SOURCE_DIRECTORY
cd $BUILD_DIRECTORY
cmake -D CMAKE_INSTALL_PREFIX=$INSTALL_DIRECTORY $SOURCE_DIRECTORY
make all test install
```

… where `$SOURCE_DIRECTORY`, `$BUILD_DIRECTORY`, and `$INSTALL_DIRECTORY` are the places where `units` is cloned, built, and installed to, respectively.  While these can all be the same location, this is not recommended as build and install outputs may interfere with (overwrite) the cloned source code.

Then, to use `units` in your own CMake project, add the `lib/cmake/units` under your `units` install location to your project's [`CMAKE_PREFIX_PATH`](https://cmake.org/cmake/help/latest/variable/CMAKE_PREFIX_PATH.html) variable.  Then, in your project's `CMakeLists.txt`:

```cmake
FIND_PACKAGE( JadeMatrix::units 0.2 REQUIRED COMPONENTS units )
```

There will now be an imported target by the name `JadeMatrix::units::units` which you can pass to [`TARGET_LINK_LIBRARIES()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html).

## Usage

`units` follows the `<author>::<library>::` namespacing convention, which is intended to be aliased to simply `units::` in most cases and unambiguous if required.  Currently, a few linear and angular units are provided in their own headers; a full set of standard units are planned by v1.0.

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
