#pragma once
#ifndef UNITS_UNIT_HPP
#define UNITS_UNIT_HPP


#include <string>


namespace units // Base unit classes ///////////////////////////////////////////
{
    template< typename T, class Traits > class unit;
    
    template<
        typename T,
        class Numer_Traits,
        class Denom_Traits
    > struct per;
    
    template<
        typename T,
        class Traits_A,
        class Traits_B
    > struct by;
    
    
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
    };
    
    template<
        typename T,
        class Numer_Traits,
        class Denom_Traits
    > struct per_traits
    {
        static std::string unit_string()
        {
            // TODO: recursive specialization for other `per_traits`s and
            // `by_traits`s to reduce the number of parentheses needed?
            return (
                "("
                + Numer_Traits::unit_string()
                + ")/("
                + Denom_Traits::unit_string()
                + ")"
            );
        }
        
        template<
            typename O,
            class Other_Numer_Traits,
            class Other_Denom_Traits
        >
        static constexpr
        T convert_from(
            const unit<O, per_traits< O, Other_Numer_Traits, Other_Denom_Traits > >& from
        )
        {
            return (
                Numer_Traits::convert_from(
                    unit< O, Other_Numer_Traits >( ( O )from )
                ) /
                Denom_Traits::convert_from(
                    unit< O, Other_Denom_Traits >( 1 )
                )
            );
        }
    };
    
    template<
        typename T,
        class Left_Traits,
        class Right_Traits
    > struct by_traits
    {
        static std::string unit_string()
        {
            // TODO: recursive specialization for other `per_traits`s and
            // `by_traits`s to reduce the number of parentheses needed?
            return (
                "("
                + Left_Traits::unit_string()
                + ")*("
                + Right_Traits::unit_string()
                + ")"
            );
        }
        
        template<
            typename O,
            class Other_Left_Traits,
            class Other_Right_Traits
        >
        static constexpr
        T convert_from(
            const unit<O, by_traits< O, Other_Left_Traits, Other_Right_Traits > >& from
        )
        {
            return (
                Left_Traits::convert_from(
                    unit< O, Other_Left_Traits >( ( O )from )
                ) *
                Right_Traits::convert_from(
                    unit< O, Other_Right_Traits >( 1 )
                )
            );
        }
    };
}


/* NOTE:
The pattern
    return ( L )lhs OPERATOR ( R )LTraits< R >::convert_from( rhs );
makes a conversion to the left-hand unit for the operation while keeping the
right-hand side the same value type.
*/


namespace units // Binary operators ////////////////////////////////////////////
{
    #define DEFINE_OPERATORS_FOR_BINARY_OPERAND( OPERAND ) \
    template< typename L, template< typename > class LTraits, typename R > \
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
    template< typename L, typename R, template< typename > class RTraits > \
    constexpr \
    auto operator OPERAND( const L& lhs, const unit< R, RTraits< R > >& rhs ) \
        -> unit< \
                     decltype( lhs OPERAND ( R )rhs ), \
            RTraits< decltype( lhs OPERAND ( R )rhs ) > \
        > \
    { \
        return lhs OPERAND ( R )rhs; \
    } \
     \
    template< \
        typename L, \
        template< typename > class LTraits, \
        typename R, \
        /*template< typename >*/ class RTraits \
    > \
    constexpr \
    auto operator OPERAND( \
        const unit< L, LTraits< L > >& lhs, \
        const unit< R, RTraits/*< R >*/ >& rhs \
    ) -> unit< \
                 decltype( ( L )lhs OPERAND ( R )LTraits< R >::convert_from( rhs ) ), \
        LTraits< decltype( ( L )lhs OPERAND ( R )LTraits< R >::convert_from( rhs ) ) > \
    > \
    { \
        return ( L )lhs OPERAND ( R )LTraits< R >::convert_from( rhs ); \
    }
    
    DEFINE_OPERATORS_FOR_BINARY_OPERAND( + )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND( - )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND( * )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND( / )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND( % )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND( & )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND( | )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND( ^ )
    
    #undef DEFINE_OPERATORS_FOR_BINARY_OPERAND
}


namespace units // Comparison operators ////////////////////////////////////////
{
    #define DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( OPERAND ) \
    template< typename L, class LTraits, typename R > \
    constexpr \
    auto operator OPERAND( const unit< L, LTraits >& lhs, const R& rhs ) \
        -> decltype( ( L )lhs OPERAND rhs ) \
    { \
        return ( L )lhs OPERAND rhs; \
    } \
     \
    template< typename L, typename R, class RTraits > \
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
        /*template< typename >*/ class RTraits \
    > \
    constexpr \
    auto operator OPERAND( \
        const unit< L, LTraits< L > >& lhs, \
        const unit< R, RTraits/*< R >*/ >& rhs \
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


namespace units // Assignment operators ////////////////////////////////////////
{
    #define DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( OPERAND, BASE_OPERAND ) \
    template< typename L, class LTraits, typename R > \
    unit< L, LTraits >& operator OPERAND( \
        unit< L, LTraits >& lhs, \
        const R& rhs \
    ) \
    { \
        lhs = ( L )lhs BASE_OPERAND rhs; \
        return lhs; \
    } \
     \
    template< typename L, typename R, class RTraits > \
    L& operator OPERAND( L& lhs, const unit< R, RTraits >& rhs ) \
    { \
        return lhs OPERAND ( R )rhs; \
    } \
     \
    template< \
        typename L, \
        template< typename > class LTraits, \
        typename R, \
        /*template< typename >*/ class RTraits \
    > \
    unit< L, LTraits< L > >& operator OPERAND( \
              unit< L, LTraits< L > >& lhs, \
        const unit< R, RTraits/*< R >*/ >& rhs \
    ) \
    { \
        lhs = ( L )lhs BASE_OPERAND ( R )LTraits< R >::convert_from( rhs ); \
        return lhs; \
    }
    
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( +=, + )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( -=, - )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( *=, * )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( /=, / )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( %=, % )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( &=, & )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( |=, | )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND( ^=, ^ )
    
    #undef DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND
}


namespace units // Unary operators /////////////////////////////////////////////
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


namespace units // Shift operators /////////////////////////////////////////////
{
    #define DEFINE_OPERATORS_FOR_BINARY_SHIFT_OPERAND( OPERAND ) \
    template< typename L, template< typename > class LTraits > \
    constexpr \
    auto operator OPERAND( const unit< L, LTraits< L > >& lhs, int rhs ) \
        -> unit< \
                     decltype( ( L )lhs OPERAND rhs ), \
            LTraits< decltype( ( L )lhs OPERAND rhs ) > \
        > \
    { \
        return ( L )lhs OPERAND rhs; \
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
        lhs = ( L )lhs BASE_OPERAND rhs; \
        return lhs; \
    }
    
    DEFINE_OPERATORS_FOR_ASSIGNMENT_SHIFT_OPERAND( <<=, << )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_SHIFT_OPERAND( >>=, >> )
    
    #undef DEFINE_OPERATORS_FOR_ASSIGNMENT_SHIFT_OPERAND
}


#endif
