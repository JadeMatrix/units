#pragma once
#ifndef JM_UNITS_UNIT_TESTS_MAIN_HPP
#define JM_UNITS_UNIT_TESTS_MAIN_HPP


#include <units/units.hpp>

#include <iomanip>  // std::setprecision()
#include <exception>
#include <string>
#include <sstream>
#include <utility>  // std::forward()


namespace tests
{
    class failure : private std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
        using std::runtime_error::what;
    };
    
    // Inspired by Catch2's clever `Decomposer` class
    template< typename Current = void > class decompose
    {
        template< typename T > friend class decompose;
        
    protected:
        const std::string& _file;
        const long long& _line;
        const std::string& _expression;
        Current _value;
        std::string _decomposed;
        
        template< typename In, typename String > decompose(
            const std::string& file,
            const long long& line,
            const std::string& expression,
            In&& value,
            String&& decomposed
        ) :
            _file{ file },
            _line{ line },
            _expression{ expression },
            _value{ std::forward< In >( value ) },
            _decomposed{ std::forward< String >( decomposed ) }
        {}
        
    public:
        decompose() = delete;
        decompose( const decompose& ) = delete;
        
        #define DEFINE_OPERATOR( OPERATOR ) \
        template< typename Next > auto operator OPERATOR( const Next& next ) \
            -> decompose< decltype( _value OPERATOR next ) > \
        { \
            return { \
                _file, \
                _line, \
                _expression, \
                ( _value OPERATOR next ), \
                _decomposed + " " #OPERATOR " " + ( \
                    std::stringstream{} << std::setprecision( 64 ) << next \
                ).str() \
            }; \
        }
        
        DEFINE_OPERATOR( <  )
        DEFINE_OPERATOR( <= )
        DEFINE_OPERATOR( >  )
        DEFINE_OPERATOR( >= )
        DEFINE_OPERATOR( == )
        DEFINE_OPERATOR( != )
        
        #undef DEFINE_OPERATOR
        
        const Current& value() const { return _value; }
        const std::string& what() const { return _decomposed; }
        
        void assert() const
        {
            if( !_value ) throw failure{
                "Test failed: "
                + _file
                + ":"
                + std::to_string( _line )
                + ": "
                + _expression
                + ", evaluated: "
                + _decomposed
            };
        }
    };
    
    template<> class decompose< void >
    {
    protected:
        std::string _file;
        long long _line;
        std::string _expression;
        
    public:
        decompose() = delete;
        decompose( const decompose& ) = delete;
        
        template< typename String1, typename String2 > decompose(
            String1&& file,
            long long line,
            String2&& expression
        ) :
            _file{ std::forward< String1 >( file ) },
            _line{ line },
            _expression{ std::forward< String2 >( expression ) }
        {}
        
        template< typename Next > decompose< Next > operator<=(
            const Next& next
        )
        {
            return {
                _file,
                _line,
                _expression,
                next,
                ( std::stringstream{} << std::setprecision( 64 ) << next ).str()
            };
        }
    };
};


#define CHECK( EXPRESSION ) ( ::tests::decompose<>{ \
    __FILE__, \
    __LINE__, \
    #EXPRESSION \
} <= EXPRESSION ).assert()


void test();


#endif
