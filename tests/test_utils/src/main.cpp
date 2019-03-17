#include <main.hpp>

#include <iostream>


int main( int, char*[] )
{
    try
    {
        test();
        return 0;
    }
    catch( const ::tests::failure& f )
    {
        std::cout << f.what() << std::endl;
        return 1;
    }
}
