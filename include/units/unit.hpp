#pragma once
#ifndef JM_UNITS_UNIT_HPP
#define JM_UNITS_UNIT_HPP


#include <ostream>
#include <string>
#include <type_traits>


namespace JadeMatrix { namespace units // Base unit classes ////////////////////
{
    template< typename T, class Traits > class unit
    {
    public:
        using  value_type = T;
        using traits_type = Traits;
        
    protected:
        value_type value;
        
        using this_type = unit< T, Traits >;
        
        template< typename... Ts > using void_t = void;
        
        template<
            typename O,
            typename Other_Traits,
            typename = void
        > struct has_convert_to : std::false_type {};
        template<
            typename O,
            typename Other_Traits
        > struct has_convert_to<
            O,
            Other_Traits,
            void_t< decltype( Other_Traits::template convert_to<
                value_type,
                traits_type
            >(
                std::declval< unit< O, Other_Traits > >()
            ) ) >
        > : std::true_type {};
        
        template< typename O, class Other_Traits >
        constexpr auto convert_from( const unit< O, Other_Traits >& o )
            -> typename std::enable_if<
                has_convert_to< O, Other_Traits >::value,
                value_type
            >::type
        {
            return Other_Traits::template convert_to< T, Traits >( o );
        }
        template< typename O, class Other_Traits >
        constexpr auto convert_from( const unit< O, Other_Traits >& o )
            -> typename std::enable_if<
                !has_convert_to< O, Other_Traits >::value,
                value_type
            >::type
        {
            return traits_type::convert_from( o );
        }
        
    public:
        constexpr unit() {}
        constexpr unit( const T& v ) : value( v ) {}
        
        // template< typename O > unit(
        //     const unit< O, Traits >& o
        // ) : value( static_cast< T >( o ) ) {}
        
        template< typename O, class Other_Traits >
        constexpr
        unit( const unit< O, Other_Traits >& o ) :
            value( convert_from( o ) )
        {}
        
        explicit constexpr operator T () const
        {
            return value;
        }
        template< typename O > explicit constexpr operator O () const
        {
            return static_cast< O >( value );
        }
        template< typename O, class Other_Traits >
        constexpr
        operator unit< O, Other_Traits >() const
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
                    unit< O, Other_Numer_Traits >( static_cast< O >( o ) )
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
            return static_cast< O >( value );
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
            return static_cast< O >( value );
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
        > constexpr auto operator *(
            const by< O, Other_Traits< O >... >& rhs
        ) const -> by<
            decltype( value * static_cast< O >( rhs ) ),
            First_Traits,
            More_Traits...,
            Other_Traits< decltype( value * static_cast< O >( rhs ) ) >...
        >
        {
            return value * static_cast< O >( rhs );
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
            return static_cast< O >( value );
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
        > constexpr auto operator *(
            const by< O, Other_Traits< O >... >& rhs
        ) const -> by<
            decltype( value * static_cast< O >( rhs ) ),
            First_Traits,
            Second_Traits,
            Other_Traits< decltype( value * static_cast< O >( rhs ) ) >...
        >
        {
            return value * static_cast< O >( rhs );
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
        ) : value( static_cast< O >( o ) )
        {}
        
        // Ratio can be implicitly converted to its base type
        constexpr operator T () const
        {
            return value;
        }
        template< typename O > explicit constexpr operator O () const
        {
            return static_cast< O >( value );
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
} }


/* NOTE:
The pattern
    return (
        static_cast< L >( lhs )
        OPERAND
        static_cast< R >( LTraits< R >::convert_from( rhs ) )
    );
makes a conversion to the left-hand unit for the operation while keeping the
right-hand side the same value type.
*/


namespace JadeMatrix { namespace units // Basic binary operators ///////////////
{
    #define DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( OPERAND ) \
    template< \
        typename L, \
        template< typename > class LTraits, \
        typename R, \
        typename std::enable_if< \
            std::is_arithmetic< R >::value, \
            int \
        >::type = 0 \
    > \
    constexpr \
    auto operator OPERAND( const unit< L, LTraits< L > >& lhs, const R& rhs ) \
        -> unit< \
                     decltype( static_cast< L >( lhs ) OPERAND rhs ), \
            LTraits< decltype( static_cast< L >( lhs ) OPERAND rhs ) > \
        > \
    { \
        return static_cast< L >( lhs ) OPERAND rhs; \
    } \
     \
    template< \
        typename L, \
        typename R, \
        template< typename > class RTraits, \
        typename std::enable_if< \
            std::is_arithmetic< L >::value, \
            int \
        >::type = 0 \
    > \
    constexpr \
    auto operator OPERAND( const L& lhs, const unit< R, RTraits< R > >& rhs ) \
        -> unit< \
                     decltype( lhs OPERAND static_cast< R >( rhs ) ), \
            RTraits< decltype( lhs OPERAND static_cast< R >( rhs ) ) > \
        > \
    { \
        return lhs OPERAND static_cast< R >( rhs ); \
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
        decltype( \
            static_cast< L >( lhs ) \
            OPERAND \
            static_cast< R >( LTraits< R >::convert_from( rhs ) ) \
        ), \
        LTraits< decltype( \
            static_cast< L >( lhs ) \
            OPERAND \
            static_cast< R >( LTraits< R >::convert_from( rhs ) ) \
        ) > \
    > \
    { \
        return ( \
            static_cast< L >( lhs ) \
            OPERAND \
            static_cast< R >( LTraits< R >::convert_from( rhs ) ) \
        ); \
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
} }


namespace JadeMatrix { namespace units // Mult. & div. specializations /////////
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
                 decltype( static_cast< L >( lhs ) * static_cast< R >( rhs ) ),
        LTraits< decltype( static_cast< L >( lhs ) * static_cast< R >( rhs ) ) >,
        RTraits
    >
    {
        return static_cast< L >( lhs ) * static_cast< R >( rhs );
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
                  decltype( static_cast< L >( lhs ) * static_cast< R >( rhs ) ),
        LTraits<  decltype( static_cast< L >( lhs ) * static_cast< R >( rhs ) ) >,
        RTraits<  decltype( static_cast< L >( lhs ) * static_cast< R >( rhs ) ) >...
    >
    {
        return static_cast< L >( lhs ) * static_cast< R >( rhs );
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
                 decltype( static_cast< L >( lhs ) * static_cast< R >( rhs ) ),
        LTraits< decltype( static_cast< L >( lhs ) * static_cast< R >( rhs ) ) >...,
        RTraits< decltype( static_cast< L >( lhs ) * static_cast< R >( rhs ) ) >
    >
    {
        return static_cast< L >( lhs ) * static_cast< R >( rhs );
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
    ) -> ratio< decltype( static_cast< L >( lhs ) * static_cast< R >( rhs ) ) >
    {
        return static_cast< L >( lhs ) * static_cast< R >( rhs );
    }
    
    template< // Dividing two ratios produces a ratio //////////////////////////
        typename L,
        typename R
    >
    constexpr
    auto operator /(
        const ratio< L >& lhs,
        const ratio< R >& rhs
    ) -> ratio< decltype( static_cast< L >( lhs ) / static_cast< R >( rhs ) ) >
    {
        return static_cast< L >( lhs ) / static_cast< R >( rhs );
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
                 decltype( static_cast< L >( lhs ) / static_cast< R >( rhs ) ),
        LTraits< decltype( static_cast< L >( lhs ) / static_cast< R >( rhs ) ) >,
        RTraits
    >
    {
        return static_cast< L >( lhs ) / static_cast< R >( rhs );
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
    ) -> ratio< decltype( static_cast< L >( lhs ) / static_cast< R >( rhs ) ) >
    {
        return static_cast< L >( lhs ) / static_cast< R >( rhs );
    }
    
    // TODO: by division, per division, per multiplication
} }


// TODO: Comparison operators for ratios (bys and pers may be a bit hard...)
namespace JadeMatrix { namespace units // Comparison operators /////////////////
{
    #define DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( OPERAND ) \
    template< \
        typename L, \
        class LTraits, \
        typename R, \
        typename std::enable_if< \
            std::is_arithmetic< R >::value, \
            int \
        >::type = 0 \
    > \
    constexpr \
    auto operator OPERAND( const unit< L, LTraits >& lhs, const R& rhs ) \
        -> decltype( static_cast< L >( lhs ) OPERAND rhs ) \
    { \
        return static_cast< L >( lhs ) OPERAND rhs; \
    } \
     \
    template< \
        typename L, \
        typename R, \
        class RTraits, \
        typename std::enable_if< \
            std::is_arithmetic< L >::value, \
            int \
        >::type = 0 \
    > \
    constexpr \
    auto operator OPERAND( const L& lhs, const unit< R, RTraits >& rhs ) \
        -> decltype( lhs OPERAND static_cast< R >( rhs ) ) \
    { \
        return lhs OPERAND static_cast< R >( rhs ); \
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
    ) -> decltype( \
        static_cast< L >( lhs ) \
        OPERAND \
        static_cast< R >( LTraits< R >::convert_from( rhs ) ) \
    ) \
    { \
        return ( \
            static_cast< L >( lhs ) \
            OPERAND \
            static_cast< R >( LTraits< R >::convert_from( rhs ) ) \
        ); \
    }
    
    DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( == )
    DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( != )
    DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( <  )
    DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( >  )
    DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( <= )
    DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( >= )
    
    #undef DEFINE_OPERATORS_FOR_COMPARISON_OPERAND
} }


namespace JadeMatrix { namespace units // Assignment operators /////////////////
{
    #define DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( OPERAND ) \
    template< \
        typename L, \
        class LTraits, \
        typename R, \
        typename std::enable_if< \
            std::is_arithmetic< R >::value, \
            int \
        >::type = 0 \
    > \
    unit< L, LTraits >& operator OPERAND##=( \
        unit< L, LTraits >& lhs, \
        const R& rhs \
    ) \
    { \
        lhs = static_cast< L >( lhs ) OPERAND rhs; \
        return lhs; \
    } \
     \
    template< \
        typename L, \
        typename R, \
        class RTraits, \
        typename std::enable_if< \
            std::is_arithmetic< L >::value, \
            int \
        >::type = 0 \
    > \
    L& operator OPERAND##=( L& lhs, const unit< R, RTraits >& rhs ) \
    { \
        return lhs OPERAND##= static_cast< R >( rhs ); \
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
        lhs = static_cast< L >( lhs ) OPERAND static_cast< R >( rhs ); \
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
} }


// TODO: unaries for per/by/ratio
namespace JadeMatrix { namespace units // Unary operators //////////////////////
{
    #define DEFINE_OPERATORS_FOR_UNARY_OPERAND( OPERAND ) \
    template< typename R, template< typename > class RTraits > \
    constexpr \
    auto operator OPERAND( const unit< R, RTraits< R > >& rhs ) \
        -> unit< \
                     decltype( OPERAND static_cast< R >( rhs ) ), \
            RTraits< decltype( OPERAND static_cast< R >( rhs ) ) > \
        > \
    { \
        return OPERAND static_cast< R >( rhs ); \
    }
    
    DEFINE_OPERATORS_FOR_UNARY_OPERAND( + )
    DEFINE_OPERATORS_FOR_UNARY_OPERAND( - )
    DEFINE_OPERATORS_FOR_UNARY_OPERAND( ~ )
    
    #undef DEFINE_OPERATORS_FOR_UNARY_OPERAND
} }


#if 0
namespace JadeMatrix { namespace units // Shift operators //////////////////////
{
    #define DEFINE_OPERATORS_FOR_BINARY_SHIFT_OPERAND( OPERAND ) \
    template< typename L, template< typename > class LTraits > \
    constexpr \
    auto operator OPERAND( const unit< L, LTraits< L > >& lhs, int rhs ) \
        -> unit< \
                     decltype( static_cast< L >( lhs ) OPERAND rhs ), \
            LTraits< decltype( static_cast< L >( lhs ) OPERAND rhs ) > \
        > \
    { \
        return static_cast< L >( lhs ) OPERAND rhs; \
    }
    
    DEFINE_OPERATORS_FOR_BINARY_SHIFT_OPERAND( << )
    DEFINE_OPERATORS_FOR_BINARY_SHIFT_OPERAND( >> )
    
    #undef DEFINE_OPERATORS_FOR_BINARY_SHIFT_OPERAND
    
    #define DEFINE_OPERATORS_FOR_ASSIGNMENT_SHIFT_OPERAND( \
        OPERAND, \
        BASE_OPERAND \
    ) \
    template< typename L, class LTraits > \
    unit< L, LTraits >& operator OPERAND( unit< L, LTraits >& lhs, int rhs ) \
    { \
        lhs = static_cast< L >( lhs ) BASE_OPERAND rhs; \
        return lhs; \
    }
    
    DEFINE_OPERATORS_FOR_ASSIGNMENT_SHIFT_OPERAND( <<=, << )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_SHIFT_OPERAND( >>=, >> )
    
    #undef DEFINE_OPERATORS_FOR_ASSIGNMENT_SHIFT_OPERAND
} }
#endif


namespace JadeMatrix { namespace units // Scalar operations specializations ////
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
} }


namespace JadeMatrix { namespace units // Stream formatting operators //////////
{
    template< typename T, class Traits >
    std::ostream& operator<<( std::ostream& out, const unit< T, Traits >& u )
    {
        return out << static_cast< T >( u ) << unit< T, Traits >::unit_symbol();
    }
    
    template< typename T, class Numer_Traits, class Denom_Traits >
    std::ostream& operator<<(
        std::ostream& out,
        const per< T, Numer_Traits, Denom_Traits >& u
    )
    {
        return out
            << static_cast< T >( u )
            << per< T, Numer_Traits, Denom_Traits >::unit_symbol()
        ;
    }
    
    template< typename T, class... Traits >
    std::ostream& operator<<( std::ostream& out, const by< T, Traits... >& u )
    {
        return out
            << static_cast< T >( u )
            << by< T, Traits... >::unit_symbol()
        ;
    }
    
    template< typename T >
    std::ostream& operator<<( std::ostream& out, const ratio< T >& u )
    {
        return out << static_cast< T >( u );
    }
} }


#endif
