#pragma once
#ifndef YAVSG_UNITS_UNIT_HPP
#define YAVSG_UNITS_UNIT_HPP


#include <ostream>
#include <string>
#include <type_traits>

// #include "scalar_operations.hpp"


namespace yavsg // Base unit classes ///////////////////////////////////////////
{
    template< typename T, class Traits > class unit
    {
    public:
        using  value_type = T;
        using traits_type = Traits;
        
    protected:
        value_type value;
        
    public:
        constexpr unit() {}
        constexpr unit( const T& v ) : value( v ) {}
        
        // template< typename O > unit(
        //     const unit< O, Traits >& o
        // ) : value( ( T )o ) {}
        
        template< typename O, class Other_Traits >
        constexpr
        unit( const unit< O, Other_Traits >& o ) :
            value( traits_type::convert_from( o ) )
        {}
        
        explicit constexpr operator T () const
        {
            return value;
        }
        template< typename O > explicit constexpr operator O () const
        {
            return ( O )value;
        }
        template< typename O, class Other_Traits >
        constexpr
        operator unit< O, Other_Traits >()
        {
            return Other_Traits::convert_from( *this );
        }
        
        static std::string unit_name()
        {
            return Traits::unit_name();
        }
        static std::string unit_symbol()
        {
            return Traits::unit_symbol();
        }
    };
    
    template< typename T, class Numer_Traits, class Denom_Traits > class per
    {
    public:
        using value_type = T;
        using numer_traits_type = Numer_Traits;
        using denom_traits_type = Denom_Traits;
        
    protected:
        value_type value;
        
    public:
        constexpr per() {}
        constexpr per( const T& v ) : value( v ) {}
        
        template<
            typename O,
            class Other_Numer_Traits,
            class Other_Denom_Traits
        >
        constexpr
        per( const per< O, Other_Numer_Traits, Other_Denom_Traits >& o ) :
            value(
                Numer_Traits::convert_from(
                    unit< O, Other_Numer_Traits >( ( O )o )
                ) /
                Denom_Traits::convert_from(
                    unit< O, Other_Denom_Traits >( 1 )
                )
            )
        {}
        
        explicit constexpr operator T () const
        {
            return value;
        }
        template< typename O > explicit constexpr operator O () const
        {
            return ( O )value;
        }
        
        static std::string unit_name()
        {
            return (
                "("
                + Numer_Traits::unit_name()
                + ")/("
                + Denom_Traits::unit_name()
                + ")"
            );
        }
        static std::string unit_symbol()
        {
            return (
                "("
                + Numer_Traits::unit_symbol()
                + ")/("
                + Denom_Traits::unit_symbol()
                + ")"
            );
        }
    };
    
    template<
        typename T,
        class First_Traits,
        class Second_Traits,
        class... More_Traits
    > class by
    {
    public:
        using         value_type = T;
        using  first_traits_type = First_Traits;
        using second_traits_type = Second_Traits;
        
    protected:
        value_type value;
        
    public:
        constexpr by() {}
        constexpr by( const T& v ) : value( v ) {}
        
        explicit constexpr operator T () const
        {
            return value;
        }
        template< typename O > explicit constexpr operator O () const
        {
            return ( O )value;
        }
        
        static std::string unit_name_raw()
        {
            return (
                unit< T, First_Traits >::unit_name()
                + std::string( "*" )
                + by< T, Second_Traits, More_Traits... >::unit_name_raw()
            );
        }
        static std::string unit_symbol_raw()
        {
            return (
                unit< T, First_Traits >::unit_symbol()
                + std::string( "*" )
                + by< T, Second_Traits, More_Traits... >::unit_symbol_raw()
            );
        }
        
        static std::string unit_name()
        {
            return "(" + unit_name_raw() + ")";
        }
        static std::string unit_symbol()
        {
            return "(" + unit_symbol_raw() + ")";
        }
        
        template< // Multiplying two bys produces a by /////////////////////////
            typename O,
            template< typename > class... Other_Traits
        >
        constexpr
        auto operator *(
            const   by< O, Other_Traits< O >... >& rhs
        ) -> by<
            decltype( value * ( O )rhs ),
            First_Traits,
            More_Traits...,
            Other_Traits< decltype( value * ( O )rhs ) >...
        >
        {
            return value * ( O )rhs;
        }
    };
    
    // A by must have at least two traits types
    template<
        typename T,
        class First_Traits,
        class Second_Traits
    > class by< T, First_Traits, Second_Traits >
    {
    public:
        using         value_type = T;
        using  first_traits_type = First_Traits;
        using second_traits_type = Second_Traits;
        
    protected:
        value_type value;
        
    public:
        constexpr by() {}
        constexpr by( const T& v ) : value( v ) {}
        
        explicit constexpr operator T () const
        {
            return value;
        }
        template< typename O > explicit constexpr operator O () const
        {
            return ( O )value;
        }
        
        static std::string unit_name_raw()
        {
            return (
                unit< T, First_Traits >::unit_name()
                + std::string( "*" )
                + unit< T, Second_Traits >::unit_name()
            );
        }
        static std::string unit_symbol_raw()
        {
            return (
                unit< T, First_Traits >::unit_symbol()
                + std::string( "*" )
                + unit< T, Second_Traits >::unit_symbol()
            );
        }
        
        static std::string unit_name()
        {
            return "(" + unit_name_raw() + ")";
        }
        static std::string unit_symbol()
        {
            return "(" + unit_symbol_raw() + ")";
        }
        
        template< // Multiplying two bys produces a by /////////////////////////
            typename O,
            template< typename > class... Other_Traits
        >
        constexpr
        auto operator *(
            const   by< O, Other_Traits< O >... >& rhs
        ) -> by<
            decltype( value * ( O )rhs ),
            First_Traits,
            Second_Traits,
            Other_Traits< decltype( value * ( O )rhs ) >...
        >
        {
            return value * ( O )rhs;
        }
    };
    
    template< typename T > class ratio
    {
    public:
        using value_type = T;
        
    protected:
        value_type value;
        
    public:
        constexpr ratio() {}
        constexpr ratio( const T& v ) : value( v ) {}
        
        template< typename O, class Other_Traits > constexpr ratio(
            const per< O, Other_Traits, Other_Traits >& o
        ) : value( ( O )o )
        {}
        
        // Ratio can be implicitly converted to its base type
        constexpr operator T () const
        {
            return value;
        }
        template< typename O > explicit constexpr operator O () const
        {
            return ( O )value;
        }
        
        static std::string unit_name()
        {
            return "";
        }
        static std::string unit_symbol()
        {
            return "";
        }
    };
}


/* NOTE:
The pattern
    return ( L )lhs OPERATOR ( R )LTraits< R >::convert_from( rhs );
makes a conversion to the left-hand unit for the operation while keeping the
right-hand side the same value type.
*/


namespace yavsg // Basic binary operators //////////////////////////////////////
{
    #define DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( OPERAND ) \
    template< \
        typename L, \
        template< typename > class LTraits, \
        typename R, \
        typename std::enable_if< std::is_arithmetic< R >::value, R >::type = 0 \
    > \
    constexpr \
    auto operator OPERAND( const unit< L, LTraits< L > >& lhs, const R& rhs ) \
        -> unit< \
                     decltype( ( L )lhs OPERAND rhs ), \
            LTraits< decltype( ( L )lhs OPERAND rhs ) > \
        > \
    { \
        return ( L )lhs OPERAND rhs; \
    } \
     \
    template< \
        typename L, \
        typename R, \
        template< typename > class RTraits, \
        typename std::enable_if< std::is_arithmetic< L >::value, L >::type = 0 \
    > \
    constexpr \
    auto operator OPERAND( const L& lhs, const unit< R, RTraits< R > >& rhs ) \
        -> unit< \
                     decltype( lhs OPERAND ( R )rhs ), \
            RTraits< decltype( lhs OPERAND ( R )rhs ) > \
        > \
    { \
        return lhs OPERAND ( R )rhs; \
    }
    
    #define DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( OPERAND ) \
    template< \
        typename L, \
        template< typename > class LTraits, \
        typename R, \
        class RTraits \
    > \
    constexpr \
    auto operator OPERAND( \
        const unit< L, LTraits< L > >& lhs, \
        const unit< R, RTraits      >& rhs \
    ) -> unit< \
                 decltype( ( L )lhs OPERAND ( R )LTraits< R >::convert_from( rhs ) ), \
        LTraits< decltype( ( L )lhs OPERAND ( R )LTraits< R >::convert_from( rhs ) ) > \
    > \
    { \
        return ( L )lhs OPERAND ( R )LTraits< R >::convert_from( rhs ); \
    }
    
    DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( + )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( - )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( * )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( / )
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( % )
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( & )
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( | )
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( ^ )
    
    DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( + )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( - )
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( * ) Special case below
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( / ) Special case below
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( % ) No way to preserve units
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( & ) No way to preserve units
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( | ) No way to preserve units
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( ^ ) No way to preserve units
    
    #undef DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT
    #undef DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT
}


namespace yavsg // Multiplication & division specializations ///////////////////
{
    template< // Multiplying two units produces a by ///////////////////////////
        typename L,
        template< typename > class LTraits,
        typename R,
        class RTraits
    >
    constexpr
    auto operator *(
        const unit< L, LTraits< L > >& lhs,
        const unit< R, RTraits      >& rhs
    ) -> by<
                 decltype( ( L )lhs * ( R )rhs ),
        LTraits< decltype( ( L )lhs * ( R )rhs ) >,
        RTraits
    >
    {
        return ( L )lhs * ( R )rhs;
    }
    
    template< // Multiplying a unit and a by produces a by /////////////////////
        typename L,
        template< typename > class LTraits,
        typename R,
        template< typename > class... RTraits
    >
    constexpr
    auto operator *(
        const unit< L, LTraits< L >    >& lhs,
        const   by< R, RTraits< R >... >& rhs
    ) -> by<
                  decltype( ( L )lhs * ( R )rhs ),
        LTraits<  decltype( ( L )lhs * ( R )rhs ) >,
        RTraits<  decltype( ( L )lhs * ( R )rhs ) >...
    >
    {
        return ( L )lhs * ( R )rhs;
    }
    
    template< // Multiplying a by and a unit produces a by /////////////////////
        typename L,
        template< typename > class... LTraits,
        typename R,
        template< typename > class RTraits
    >
    constexpr
    auto operator *(
        const   by< L, LTraits< L >... >& lhs,
        const unit< R, RTraits< R >    >& rhs
    ) -> by<
                 decltype( ( L )lhs * ( R )rhs ),
        LTraits< decltype( ( L )lhs * ( R )rhs ) >...,
        RTraits< decltype( ( L )lhs * ( R )rhs ) >
    >
    {
        return ( L )lhs * ( R )rhs;
    }
    
    // Multiplying two bys produces a by -- implemented in class ///////////////
    
    template< // Multiplying two ratios produces a ratio ///////////////////////
        typename L,
        typename R
    >
    constexpr
    auto operator *(
        const ratio< L >& lhs,
        const ratio< R >& rhs
    ) -> ratio< decltype( ( L )lhs * ( R )rhs ) >
    {
        return ( L )lhs * ( R )rhs;
    }
    
    template< // Dividing two units produces a per /////////////////////////////
        typename L,
        template< typename > class LTraits,
        typename R,
        class RTraits
    >
    constexpr
    auto operator /(
        const unit< L, LTraits< L > >& lhs,
        const unit< R, RTraits      >& rhs
    ) -> per<
                 decltype( ( L )lhs / ( R )rhs ),
        LTraits< decltype( ( L )lhs / ( R )rhs ) >,
        RTraits
    >
    {
        return ( L )lhs / ( R )rhs;
    }
    
    template< // Dividing two of the same unit produces a ratio unit ///////////
        typename L,
        typename R,
        template< typename > class Traits
    >
    constexpr
    auto operator /(
        const unit< L, Traits< L > >& lhs,
        const unit< R, Traits< R > >& rhs
    ) -> ratio< decltype( ( L )lhs / ( R )rhs ) >
    {
        return ( L )lhs / ( R )rhs;
    }
    
    // TODO: by division, per division, per multiplication
}


// TODO: Comparison operators for ratios (bys and pers may be a bit hard...)
namespace yavsg // Comparison operators ////////////////////////////////////////
{
    #define DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( OPERAND ) \
    template< \
        typename L, \
        class LTraits, \
        typename R, \
        typename std::enable_if< std::is_arithmetic< R >::value, R >::type = 0 \
    > \
    constexpr \
    auto operator OPERAND( const unit< L, LTraits >& lhs, const R& rhs ) \
        -> decltype( ( L )lhs OPERAND rhs ) \
    { \
        return ( L )lhs OPERAND rhs; \
    } \
     \
    template< \
        typename L, \
        typename R, \
        class RTraits, \
        typename std::enable_if< std::is_arithmetic< L >::value, L >::type = 0 \
    > \
    constexpr \
    auto operator OPERAND( const L& lhs, const unit< R, RTraits >& rhs ) \
        -> decltype( lhs OPERAND ( R )rhs ) \
    { \
        return lhs OPERAND ( R )rhs; \
    } \
     \
    template< \
        typename L, \
        template< typename > class LTraits, \
        typename R, \
        class RTraits \
    > \
    constexpr \
    auto operator OPERAND( \
        const unit< L, LTraits< L > >& lhs, \
        const unit< R, RTraits      >& rhs \
    ) -> decltype( ( L )lhs OPERAND ( R )LTraits< R >::convert_from( rhs ) ) \
    { \
        return ( L )lhs OPERAND ( R )LTraits< R >::convert_from( rhs ); \
    }
    
    DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( == )
    DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( != )
    DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( <  )
    DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( >  )
    DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( <= )
    DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( >= )
    
    #undef DEFINE_OPERATORS_FOR_COMPARISON_OPERAND
}


namespace yavsg // Assignment operators ////////////////////////////////////////
{
    #define DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( OPERAND ) \
    template< \
        typename L, \
        class LTraits, \
        typename R, \
        typename std::enable_if< std::is_arithmetic< R >::value, R >::type = 0 \
    > \
    unit< L, LTraits >& operator OPERAND##=( \
        unit< L, LTraits >& lhs, \
        const R& rhs \
    ) \
    { \
        lhs = ( L )lhs OPERAND rhs; \
        return lhs; \
    } \
     \
    template< \
        typename L, \
        typename R, \
        class RTraits, \
        typename std::enable_if< std::is_arithmetic< L >::value, L >::type = 0 \
    > \
    L& operator OPERAND##=( L& lhs, const unit< R, RTraits >& rhs ) \
    { \
        return lhs OPERAND##= ( R )rhs; \
    } \
     \
    template< \
        typename L, \
        class LTraits, \
        typename R \
    > \
    unit< L, LTraits >& operator OPERAND##=( \
        unit< L, LTraits >& lhs, \
        const ratio< R >& rhs \
    ) \
    { \
        lhs = ( L )lhs OPERAND ( R )rhs; \
        return lhs; \
    } \
    
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( + )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( - )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( * )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( / )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( % )
    // DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( & )
    // DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( | )
    // DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( ^ )
    
    #undef DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND
}


// TODO: unaries for per/by/ratio
namespace yavsg // Unary operators /////////////////////////////////////////////
{
    #define DEFINE_OPERATORS_FOR_UNARY_OPERAND( OPERAND ) \
    template< typename R, template< typename > class RTraits > \
    constexpr \
    auto operator OPERAND( const unit< R, RTraits< R > >& rhs ) \
        -> unit< \
                     decltype( OPERAND( R )rhs ), \
            RTraits< decltype( OPERAND( R )rhs ) > \
        > \
    { \
        return OPERAND( R )rhs; \
    }
    
    DEFINE_OPERATORS_FOR_UNARY_OPERAND( + )
    DEFINE_OPERATORS_FOR_UNARY_OPERAND( - )
    DEFINE_OPERATORS_FOR_UNARY_OPERAND( ~ )
    
    #undef DEFINE_OPERATORS_FOR_UNARY_OPERAND
}


// namespace yavsg // Shift operators /////////////////////////////////////////////
// {
//     #define DEFINE_OPERATORS_FOR_BINARY_SHIFT_OPERAND( OPERAND ) \
//     template< typename L, template< typename > class LTraits > \
//     constexpr \
//     auto operator OPERAND( const unit< L, LTraits< L > >& lhs, int rhs ) \
//         -> unit< \
//                      decltype( ( L )lhs OPERAND rhs ), \
//             LTraits< decltype( ( L )lhs OPERAND rhs ) > \
//         > \
//     { \
//         return ( L )lhs OPERAND rhs; \
//     }
    
//     DEFINE_OPERATORS_FOR_BINARY_SHIFT_OPERAND( << )
//     DEFINE_OPERATORS_FOR_BINARY_SHIFT_OPERAND( >> )
    
//     #undef DEFINE_OPERATORS_FOR_BINARY_SHIFT_OPERAND
    
//     #define DEFINE_OPERATORS_FOR_ASSIGNMENT_SHIFT_OPERAND( \
//         OPERAND, \
//         BASE_OPERAND \
//     ) \
//     template< typename L, class LTraits > \
//     unit< L, LTraits >& operator OPERAND( unit< L, LTraits >& lhs, int rhs ) \
//     { \
//         lhs = ( L )lhs BASE_OPERAND rhs; \
//         return lhs; \
//     }
    
//     DEFINE_OPERATORS_FOR_ASSIGNMENT_SHIFT_OPERAND( <<=, << )
//     DEFINE_OPERATORS_FOR_ASSIGNMENT_SHIFT_OPERAND( >>=, >> )
    
//     #undef DEFINE_OPERATORS_FOR_ASSIGNMENT_SHIFT_OPERAND
// }


namespace yavsg // Scalar operations specializations ///////////////////////////
{
    // template<
    //     typename T,
    //     class Traits,
    //     unsigned int PM
    // > sqrt( const power< T, Traits, 2 * PM >& u )
    // {
    //     return power< T, Traits, PM >( sqrt( ( T )u ) );
    // }
    
    // template<
    //     typename T,
    //     class Traits
    // > sqrt< T, Traits, 2 >( const power< T, Traits, 2 >& u )
    // {
    //     return unit< T, Traits>( sqrt( ( T )u ) );
    // }
    
    // template<
    //     typename T,
    //     class Traits
    // > sqrt< T, Traits, 0 >( const power< T, Traits, 0 >& u )
    // {
    //     return ratio< T >( sqrt( ( T )u ) );
    // }
}


namespace yavsg // Stream formatting operators /////////////////////////////////
{
    template< typename T, class Traits >
    std::ostream& operator<<( std::ostream& out, const unit< T, Traits >& u )
    {
        return out << ( T )u << unit< T, Traits >::unit_symbol();
    }
    
    template< typename T, class Numer_Traits, class Denom_Traits >
    std::ostream& operator<<(
        std::ostream& out,
        const per< T, Numer_Traits, Denom_Traits >& u
    )
    {
        return out
            << ( T )u
            << per< T, Numer_Traits, Denom_Traits >::unit_symbol()
        ;
    }
    
    template< typename T, class... Traits >
    std::ostream& operator<<( std::ostream& out, const by< T, Traits... >& u )
    {
        return out << ( T )u << by< T, Traits... >::unit_symbol();
    }
    
    template< typename T >
    std::ostream& operator<<( std::ostream& out, const ratio< T >& u )
    {
        return out << ( T )u;
    }
}


#endif
