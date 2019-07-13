#pragma once
#ifndef JM_UNITS_TEST_CUSTOM_VALUE_TYPE_HPP
#define JM_UNITS_TEST_CUSTOM_VALUE_TYPE_HPP


#include <units/core/constants.hpp>
#include <units/core/internal/utils.hpp>

#include <cmath>    // isnan, remainder
#include <limits>
#include <ostream>
#include <string>
#include <type_traits>


namespace custom
{
    template<
        typename,
        typename = void
    > struct is_tagged : std::false_type {};
    template< typename T > struct is_tagged<
        T,
        ::JadeMatrix::units::internal::void_t<
            typename T::original_type,
            typename T::     tag_type
        >
    > : std::true_type {};
    
    template< typename T, typename = void > struct detag
    {
        using type = ::JadeMatrix::units::internal::remove_cvref_t< T >;
    };
    template< typename T > struct detag<
        T,
        typename std::enable_if< is_tagged< T >::value >::type
    >
    {
        using type = typename detag<
            ::JadeMatrix::units::internal::remove_cvref_t<
                typename T::original_type
            >
        >::type;
    };
    template< typename T > using detagged = typename detag< T >::type;
    
    template< typename T, typename Tag > class tagged
    {
    public:
        using original_type = ::JadeMatrix::units::internal::remove_cvref_t< T >;
        using      tag_type = Tag;
    private:
        original_type _value;
    public:
        constexpr tagged( const original_type& v ) : _value( v ) {};
        template< typename O, typename OtherTag > constexpr tagged(
            const tagged< O, OtherTag >& o
        ) : tagged( o.value() )
        {}
        
        constexpr original_type value() const { return _value; }
        explicit constexpr operator original_type() const { return _value; }
    };
    
    #define DEFINE_BINARY_OPERATOR( OPERAND ) \
    template< typename LHS, typename LHSTag, typename RHS, typename RHSTag > \
    constexpr auto operator OPERAND( \
        const tagged< LHS, LHSTag >& lhs, \
        const tagged< RHS, RHSTag >& rhs \
    ) -> tagged< detagged< decltype( \
        lhs.value() OPERAND rhs.value() \
    ) >, LHSTag > \
    { \
        return lhs.value() OPERAND rhs.value(); \
    } \
    template< typename LHS, typename LHSTag, typename RHS > \
    constexpr auto operator OPERAND( \
        const tagged< LHS, LHSTag >& lhs, \
        const         RHS          & rhs \
    ) -> tagged< detagged< decltype( \
        lhs.value() OPERAND rhs \
    ) >, LHSTag > \
    { \
        return lhs.value() OPERAND rhs; \
    } \
    template< typename LHS, typename RHS, typename RHSTag > \
    constexpr auto operator OPERAND( \
        const         LHS          & lhs, \
        const tagged< RHS, RHSTag >& rhs \
    ) -> tagged< detagged< decltype( \
        lhs OPERAND rhs.value() \
    ) >, RHSTag > \
    { \
        return lhs OPERAND rhs.value(); \
    }
    
    DEFINE_BINARY_OPERATOR( +  )
    DEFINE_BINARY_OPERATOR( -  )
    DEFINE_BINARY_OPERATOR( *  )
    DEFINE_BINARY_OPERATOR( /  )
    DEFINE_BINARY_OPERATOR( %  )
    DEFINE_BINARY_OPERATOR( == )
    DEFINE_BINARY_OPERATOR( != )
    DEFINE_BINARY_OPERATOR( <  )
    DEFINE_BINARY_OPERATOR( >  )
    DEFINE_BINARY_OPERATOR( <= )
    DEFINE_BINARY_OPERATOR( >= )
    
    #undef DEFINE_BINARY_OPERATOR
    
    #define DEFINE_ASSIGN_OPERATOR( OPERAND ) \
    template< typename LHS, typename LHSTag, typename RHS, typename RHSTag > \
    constexpr auto operator OPERAND##=( \
              tagged< LHS, LHSTag >& lhs, \
        const tagged< RHS, RHSTag >& rhs \
    ) -> tagged< detagged< LHS >, LHSTag >& \
    { \
        return ( \
            lhs = ( lhs.value() OPERAND rhs.value() ), \
            lhs \
        ); \
    } \
    template< typename LHS, typename LHSTag > \
    constexpr auto operator OPERAND##=( \
              tagged< LHS, LHSTag >& lhs, \
        const         LHS          & rhs \
    ) -> tagged< LHS, LHSTag >& \
    { \
        return ( \
            lhs = ( lhs.value() OPERAND rhs ), \
            lhs \
        ); \
    }
    
    DEFINE_ASSIGN_OPERATOR( + )
    DEFINE_ASSIGN_OPERATOR( - )
    DEFINE_ASSIGN_OPERATOR( * )
    DEFINE_ASSIGN_OPERATOR( / )
    DEFINE_ASSIGN_OPERATOR( % )
    
    #undef DEFINE_ASSIGN_OPERATOR
    
    #define DEFINE_UNARY_OPERATOR( OPERAND ) \
    template< typename RHS, typename RHSTag > \
    constexpr tagged< RHS, RHSTag > operator OPERAND( \
        const tagged< RHS, RHSTag >& rhs \
    ) \
    { \
        return OPERAND rhs.value(); \
    }
    
    DEFINE_UNARY_OPERATOR( + )
    DEFINE_UNARY_OPERATOR( - )
    
    #undef DEFINE_UNARY_OPERATOR
    
    template< typename T, typename Tag > std::string to_string(
        const tagged< T, Tag >& val
    )
    {
        return (
            "<"
            + to_string( val.value() )
            + ","
            + typeid( Tag ).name() // Lazy
            + ">"
        );
    }
    
    template< typename T, typename Tag > std::ostream& operator <<(
        std::ostream& out,
        const tagged< T, Tag >& val
    )
    {
        std::ostream::sentry s{ out };
        if( s ) out << to_string( val );
        return out;
    }
}


namespace custom
{
    class bar
    {
        friend std::string to_string( const bar& );
    private:
        long double _value;
        
    public:
        constexpr bar( long double v ) : _value( v ) {}
        template<
            typename T,
            typename Tag
        > constexpr bar( const tagged< T, Tag >& t ) :
            bar( t.value() )
        {}
        
        #define DEFINE_BINARY_OPERATOR( OPERAND, TAG ) \
        struct TAG##_tag {}; \
        tagged< bar, TAG##_tag > operator OPERAND( const bar& o ) const \
        { \
            return tagged< bar, TAG##_tag >( bar{ _value OPERAND o._value } ); \
        }
        
        DEFINE_BINARY_OPERATOR( *, mul )
        DEFINE_BINARY_OPERATOR( /, div )
        // DEFINE_BINARY_OPERATOR( %, mod )
        DEFINE_BINARY_OPERATOR( +, plu )
        DEFINE_BINARY_OPERATOR( -, min )
        
        struct mod_tag {};
        tagged< bar, mod_tag > operator %( const bar& o ) const
        {
            return tagged< bar, mod_tag >( bar{ std::remainder(
                _value,
                o._value
            ) } );
        }
        
        #undef DEFINE_BINARY_OPERATOR
        
        enum class cmp { invalid, yes, no };
        
        #define DEFINE_COMPARE_OPERATOR( OPERAND, TAG ) \
        struct TAG##_tag {}; \
        tagged< cmp, TAG##_tag > operator OPERAND( const bar& o ) const \
        { \
            return ( \
                std::isnan( _value ) || std::isnan( o._value ) \
                ? cmp::invalid \
                : ( \
                    _value OPERAND o._value \
                    ? cmp::yes \
                    : cmp::no \
                ) \
            ); \
        }
        
        DEFINE_COMPARE_OPERATOR( ==, eq )
        DEFINE_COMPARE_OPERATOR( !=, ne )
        DEFINE_COMPARE_OPERATOR( > , gt )
        DEFINE_COMPARE_OPERATOR( < , lt )
        DEFINE_COMPARE_OPERATOR( >=, ge )
        DEFINE_COMPARE_OPERATOR( <=, le )
        
        #undef DEFINE_COMPARE_OPERATOR
        
        #define DEFINE_ASSIGN_OPERATOR( OPERAND ) \
        bar& operator OPERAND##=( const bar& o ) \
        { \
            return ( _value OPERAND##= o._value, *this ); \
        }
        
        DEFINE_ASSIGN_OPERATOR( + )
        DEFINE_ASSIGN_OPERATOR( - )
        DEFINE_ASSIGN_OPERATOR( * )
        DEFINE_ASSIGN_OPERATOR( / )
        // DEFINE_ASSIGN_OPERATOR( % )
        
        bar& operator %=( const bar& o )
        {
            return ( _value = std::remainder( _value, o._value ), *this );
        }
        
        #undef DEFINE_ASSIGN_OPERATOR
    };
    
    constexpr auto nan = std::numeric_limits< long double >::has_quiet_NaN
        ? std::numeric_limits< long double >::    quiet_NaN()
        : std::numeric_limits< long double >::signaling_NaN()
    ;
    constexpr auto inf = std::numeric_limits< long double >::infinity();
    
    inline std::string to_string( const bar& val )
    {
        if( std::isnan( val._value ) ) return "非";
        else if( val._value ==  inf ) return "正無";
        else if( val._value == -inf ) return "負無";
        
        auto s1 = std::to_string( val._value );
        std::string s2;
        for( auto c : s1 )
        {
            switch( c )
            {
            case '0': s2 += "零"; break;
            case '1': s2 += "一"; break;
            case '2': s2 += "二"; break;
            case '3': s2 += "三"; break;
            case '4': s2 += "四"; break;
            case '5': s2 += "五"; break;
            case '6': s2 += "六"; break;
            case '7': s2 += "七"; break;
            case '8': s2 += "八"; break;
            case '9': s2 += "九"; break;
            case '.': s2 += "点"; break;
            case '+': s2 += "正"; break;
            case '-': s2 += "負"; break;
            case 'e': s2 += "冪"; break;
            default : s2 += "？"; break;
            }
        }
        return s2;
    }
    
    inline std::ostream& operator <<( std::ostream& out, const bar& val )
    {
        std::ostream::sentry s{ out };
        if( s ) out << to_string( val );
        return out;
    }
}


#endif
