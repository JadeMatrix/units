#pragma once
#ifndef UNITS_UNIT_HPP
#define UNITS_UNIT_HPP


#include <string>


namespace units // Base unit classes ///////////////////////////////////////////
{
    template< typename T, template< typename > class Traits > class unit;
    
    template<
        typename T,
        template< typename > class Numer_Traits,
        template< typename > class Denom_Traits
    > struct per;
    
    template<
        typename T,
        template< typename > class Traits_A,
        template< typename > class Traits_B
    > struct by;
    
    
    template< typename T, template< typename > class Traits > class unit
    {
    public:
        using  value_type = T;
        using traits_type = Traits< T >;
        
    protected:
        value_type value;
        
    public:
        constexpr unit() {}
        constexpr unit( const T& v ) : value( v ) {}
        
        template< typename O > unit(
            const unit< O, Traits >& o
        ) : value( ( T )o ) {}
        
        template< typename O, template< typename > class Other_Traits >
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
        template< typename > class Numer_Traits,
        template< typename > class Denom_Traits
    > struct per_traits
    {
        // using value_type = T;
        template< typename O > using numer_traits_type = Numer_Traits< O >;
        template< typename O > using denom_traits_type = Denom_Traits< O >;
        
        template<
            template< typename > class Other_Numer_Traits,
            template< typename > class Other_Denom_Traits
        > struct split_type
        {
            template< typename O > using template_type = per_traits<
                O,
                Other_Numer_Traits,
                Other_Denom_Traits
            >;
        };
        template< typename O > using template_type = per_traits<
            O,
            Numer_Traits,
            Denom_Traits
        >;
        
        static std::string unit_string()
        {
            // TODO: recursive specialization for other `per_traits`s and `by`s to
            // reduce the number of parentheses needed?
            return (
                "("
                + Numer_Traits< T >::unit_string()
                + ")/("
                + Denom_Traits< T >::unit_string()
                + ")"
            );
        }
        
        // template< typename O > static constexpr T convert_from(
        //     const unit< O, template_type >& from
        // )
        // {
        //     return ( O )from;
        // }
        
        template<
            typename O,
            template< typename > class Other_Numer_Traits,
            template< typename > class Other_Denom_Traits,
            template< typename, template< typename > class, template< typename > class > class Other_per_Traits
        >
        static constexpr
        T convert_from(
            // const per_traits< O, Other_Numer_Traits, Other_Denom_Traits >& from
            // const unit< O, template_type >& from
            // const unit< O, split_type< Other_Numer_Traits, Other_Denom_Traits >::template_type >& from
            // const unit< O, per_traits< Other_Numer_Traits, Other_Denom_Traits, O >::template_type >& from
            // const unit< O, per_traits< Other_Numer_Traits, Other_Denom_Traits > >& from
            // const unit< O, per_traits >& from
            // const unit< O, Other_per_Traits >& from
            const unit< O, split_type< Other_Numer_Traits, Other_Denom_Traits >::template_type >& from
        )
        {
            return (
                Numer_Traits< O >::convert_from(
                    // unit< O, decltype( from )::traits_type::numer_traits_type >( ( O )from )
                    // unit< O, Other_Numer_Traits >( ( O )from )
                    unit< O, Other_per_Traits< O, Other_Numer_Traits, Other_Denom_Traits >::numer_traits_type >( ( O )from )
                ) /
                Denom_Traits< O >::convert_from(
                    // unit< O, decltype( from )::traits_type::denom_traits_type >( 1 )
                    // unit< O, Other_Denom_Traits >( 1 )
                    unit< O, Other_per_Traits< O, Other_Numer_Traits, Other_Denom_Traits >::denom_traits_type >( 1 )
                )
            );
        }
    };
    
    // template<
    //     typename T,
    //     template< typename > class Traits_A,
    //     template< typename > class Traits_B
    // > struct by_traits
    // {
    //     using value_type = T;
        
    //     static std::string unit_string()
    //     {
    //         // TODO: recursive specialization for other `per`s and `by_traits`s to
    //         // reduce the number of parentheses needed?
    //         return (
    //             "("
    //             + Traits_A< T >::unit_string()
    //             + ")*("
    //             + Traits_B< T >::unit_string()
    //             + ")"
    //         );
    //     }
        
    //     template<
    //         typename O,
    //         template< typename > class Other_Traits_A,
    //         template< typename > class Other_Traits_B
    //     >
    //     static constexpr
    //     T convert_from(
    //         const by_traits< O, Other_Traits_A, Other_Traits_B >& from
    //     )
    //     {
    //         return (
    //             Traits_A< O >::convert_from(
    //                 unit< O, Other_Traits_A >( ( O )from )
    //             ) *
    //             Traits_B< O >::convert_from(
    //                 unit< O, Other_Traits_B >( 1 )
    //             )
    //         );
    //     }
    // };
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
    auto operator OPERAND( const unit< L, LTraits >& lhs, const R& rhs ) \
        -> unit< decltype( ( L )lhs OPERAND rhs ), LTraits > \
    { \
        return ( L )lhs OPERAND rhs; \
    } \
     \
    template< typename L, typename R, template< typename > class RTraits > \
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
        template< typename > class RTraits \
    > \
    constexpr \
    auto operator OPERAND( \
        const unit< L, LTraits >& lhs, \
        const unit< R, RTraits >& rhs \
    ) -> unit< \
        decltype( ( L )lhs OPERAND ( R )LTraits< R >::convert_from( rhs ) ), \
        LTraits \
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
    template< typename L, template< typename > class LTraits, typename R > \
    constexpr \
    auto operator OPERAND( const unit< L, LTraits >& lhs, const R& rhs ) \
        -> decltype( ( L )lhs OPERAND rhs ) \
    { \
        return ( L )lhs OPERAND rhs; \
    } \
     \
    template< typename L, typename R, template< typename > class RTraits > \
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
        template< typename > class RTraits \
    > \
    constexpr \
    auto operator OPERAND( \
        const unit< L, LTraits >& lhs, \
        const unit< R, RTraits >& rhs \
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
    template< typename L, template< typename > class LTraits, typename R > \
    unit< L, LTraits >& operator OPERAND( \
        unit< L, LTraits >& lhs, \
        const R& rhs \
    ) \
    { \
        lhs = ( L )lhs BASE_OPERAND rhs; \
        return lhs; \
    } \
     \
    template< typename L, typename R, template< typename > class RTraits > \
    L& operator OPERAND( L& lhs, const unit< R, RTraits >& rhs ) \
    { \
        return lhs OPERAND ( R )rhs; \
    } \
     \
    template< \
        typename L, \
        template< typename > class LTraits, \
        typename R, \
        template< typename > class RTraits \
    > \
    unit< L, LTraits >& operator OPERAND( \
              unit< L, LTraits >& lhs, \
        const unit< R, RTraits >& rhs \
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
    auto operator OPERAND( const unit< R, RTraits >& rhs ) \
        -> unit< decltype( OPERAND( R )rhs ), RTraits > \
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
    auto operator OPERAND( const unit< L, LTraits >& lhs, int rhs ) \
        -> unit< decltype( ( L )lhs OPERAND rhs ), LTraits > \
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
    template< typename L, template< typename > class LTraits > \
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
