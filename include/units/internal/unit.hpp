#pragma once
#ifndef JM_INTERNAL_UNITS_UNIT_HPP
#define JM_INTERNAL_UNITS_UNIT_HPP


#include "core_types.hpp"
#include "traits_utils.hpp"

#include <type_traits>


namespace JadeMatrix { namespace units // Basic unit class /////////////////////
{
    template<
        typename Traits,
        typename T,
        typename Scale
    > class unit
    {
    public:
        using traits_type = Traits;
        using  value_type = T;
        using  scale_type = Scale;
        template< typename O > using unit_type = unit< traits_type, O, scale_type >;
        
    protected:
        value_type _value;
        
        using convert = traits_convert< traits_type, value_type >;
        
        template<
            typename Other_Traits,
            typename O,
            typename Other_Scale
        > static constexpr value_type from(
            const unit< Other_Traits, O, Other_Scale >& o
        )
        {
            return convert::template from< Other_Traits >(
                static_cast< O >( o )
            ) * scale_type::scale( Other_Scale::unscale( 1 ) );
        }
        
    public:
        constexpr unit() {}
        constexpr unit( const value_type& v ) : _value{ v } {}
        template<
            typename Other_Traits,
            typename O,
            typename Other_Scale
        > constexpr unit( const unit< Other_Traits, O, Other_Scale >& o ) :
            _value{ from( o ) }
        {}
        // Allow custom conversions if specialization available in unit traits
        template< typename O > constexpr unit(
            const typename std::enable_if< (
                !is_unit< O >::value
                && !std::is_arithmetic< O >::value
            ), O
        >::value& v ) : _value{ traits_type::from( v ) }
        {}
        
        template<
            typename O = value_type,
            typename = typename std::enable_if< !is_unit< O >::value >::type
        > explicit constexpr operator O () const
        {
            return static_cast< O >( _value );
        }
    };
} }


namespace JadeMatrix { namespace units // Basic binary operators ///////////////
{
    #define DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( OPERAND ) \
    template< \
        typename Left_Traits, \
        typename L, \
        typename Left_Scale, \
        typename R \
    > \
    constexpr auto operator +( \
        const unit< Left_Traits, L, Left_Scale >& lhs, \
        const R& rhs \
    ) -> unit< \
        Left_Traits, \
        decltype( static_cast< L >( lhs ) OPERAND rhs ), \
        Left_Scale \
    > \
    { \
        return static_cast< L >( lhs ) OPERAND rhs; \
    } \
     \
    template< \
        typename L, \
        typename Right_Traits, \
        typename R, \
        typename Right_Scale \
    > \
    constexpr auto operator OPERAND( \
        const L& lhs, \
        const unit< Right_Traits, R, Right_Scale >& rhs \
    ) -> unit< \
        Right_Traits, \
        decltype( lhs OPERAND static_cast< R >( rhs ) ), \
        Right_Scale \
    > \
    { \
        return lhs OPERAND static_cast< R >( rhs ); \
    }
    
    #define DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( OPERAND ) \
    template< \
        typename Left_Traits, \
        typename L, \
        typename Left_Scale, \
        typename Right_Traits, \
        typename R, \
        typename Right_Scale \
    > \
    constexpr auto operator OPERAND( \
        const unit<  Left_Traits, L,  Left_Scale >& lhs, \
        const unit< Right_Traits, R, Right_Scale >& rhs \
    ) -> typename std::enable_if< \
        conversion_exists< \
            Left_Traits, \
            L, \
            Right_Traits, \
            R \
        >::value, \
        unit< \
            Left_Traits, \
            decltype( \
                static_cast< L >( lhs ) \
                OPERAND \
                static_cast< R >( rhs ) \
            ), \
            Left_Scale \
        > \
    >::type \
    { \
        return ( \
            static_cast< L >( lhs ) \
            OPERAND \
            static_cast< R >( unit< Left_Traits, R, Left_Scale >{ rhs } ) \
        ); \
    }
    
    DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( + )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( - )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( * )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( / )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( % )
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( & ) No bitwise operators
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( | ) No bitwise operators
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT( ^ ) No bitwise operators
    
    DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( + )
    DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( - )
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( * ) Special case below
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( / ) Special case below
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( % ) No way to preserve units
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( & ) No bitwise operators
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( | ) No bitwise operators
    // DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT( ^ ) No bitwise operators
    
    #undef DEFINE_OPERATORS_FOR_BINARY_OPERAND_NONUNIT
    #undef DEFINE_OPERATORS_FOR_BINARY_OPERAND_UNIT
} }


namespace JadeMatrix { namespace units // Mult. & div. specializations /////////
{
    // Multiplying two units produces a by
    template<
        typename Left_Traits,
        typename L,
        typename Left_Scale,
        typename Right_Traits,
        typename R,
        typename Right_Scale
    >
    constexpr auto operator *(
        const unit<  Left_Traits, L,  Left_Scale >& lhs,
        const unit< Right_Traits, R, Right_Scale >& rhs
    ) -> by<
        std::decay< decltype( lhs ) >::type::template unit_type,
        std::decay< decltype( rhs ) >::type::template unit_type,
        decltype( static_cast< L >( lhs ) * static_cast< R >( rhs ) )
    >
    {
        return static_cast< L >( lhs ) * static_cast< R >( rhs );
    }
    
    // Dividing two units produces a per
    template<
        typename Left_Traits,
        typename L,
        typename Left_Scale,
        typename Right_Traits,
        typename R,
        typename Right_Scale
    > constexpr auto operator /(
        const unit<  Left_Traits, L,  Left_Scale >& lhs,
        const unit< Right_Traits, R, Right_Scale >& rhs
    ) -> typename std::enable_if<
        !conversion_exists< Left_Traits, L, Right_Traits, R >::value,
        per<
            std::decay< decltype( lhs ) >::type::template unit_type,
            std::decay< decltype( rhs ) >::type::template unit_type,
            decltype( static_cast< L >( lhs ) / static_cast< R >( rhs ) )
        >
    >::type
    {
        return static_cast< L >( lhs ) / static_cast< R >( rhs );
    }
    
    // Dividing two convertible units produces a ratio
    template<
        typename Left_Traits,
        typename L,
        typename Left_Scale,
        typename Right_Traits,
        typename R,
        typename Right_Scale
    > constexpr auto operator /(
        const unit<  Left_Traits, L,  Left_Scale >& lhs,
        const unit< Right_Traits, R, Right_Scale >& rhs
    ) -> typename std::enable_if<
        conversion_exists< Left_Traits, L, Right_Traits, R >::value,
        ratio< decltype( static_cast< L >( lhs ) / static_cast< R >( rhs ) ) >
    >::type
    {
        return (
            static_cast< L >( lhs )
            / static_cast< R >( unit< Left_Traits, R, Left_Scale >{ rhs } )
        );
    }
} }


namespace JadeMatrix { namespace units // Comparison operators /////////////////
{
    #define DEFINE_OPERATORS_FOR_COMPARISON_OPERAND( OPERAND ) \
    template< \
        typename Left_Traits, \
        typename L, \
        typename Left_Scale, \
        typename Right_Traits, \
        typename R, \
        typename Right_Scale \
    > constexpr auto operator OPERAND( \
        const unit<  Left_Traits, L,  Left_Scale >& lhs, \
        const unit< Right_Traits, R, Right_Scale >& rhs \
    ) -> decltype( std::declval< L >() OPERAND std::declval< R >() ) \
    { \
        return ( \
            static_cast< L >( lhs ) \
            OPERAND \
            static_cast< R >( unit< Left_Traits, R, Left_Scale >{ rhs } ) \
        ); \
    } \
     \
    template< \
        typename Left_Traits, \
        typename L, \
        typename Left_Scale, \
        typename R \
    > constexpr auto operator OPERAND( \
        const unit< Left_Traits, L, Left_Scale >& lhs, \
        const R                                 & rhs \
    ) -> typename std::enable_if< \
        !is_basic_unit< R >::value, \
        decltype( std::declval< L >() OPERAND std::declval< R >() ) \
    >::type \
    { \
        return ( \
            static_cast< L >( lhs ) \
            OPERAND \
            static_cast< L >( unit< Left_Traits, L, Left_Scale >{ rhs } ) \
        ); \
    } \
     \
    template< \
        typename L, \
        typename Right_Traits, \
        typename R, \
        typename Right_Scale \
    > constexpr auto operator OPERAND( \
        const L                                   & lhs, \
        const unit< Right_Traits, R, Right_Scale >& rhs \
    ) -> typename std::enable_if< \
        !is_basic_unit< L >::value, \
        decltype( std::declval< L >() OPERAND std::declval< R >() ) \
    >::type \
    { \
        return ( \
            static_cast< R >( unit< Right_Traits, R, Right_Scale >{ lhs } ) \
            OPERAND \
            static_cast< R >( rhs ) \
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
    #define DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_NONUNIT( OPERAND ) \
    template< \
        typename Left_Traits, \
        typename L, \
        typename Left_Scale, \
        typename R \
    > constexpr auto operator OPERAND##=( \
              unit< Left_Traits, L, Left_Scale >& lhs, \
        const R                                 & rhs \
    ) -> typename std::enable_if< \
        !is_basic_unit< R >::value, \
        decltype( lhs )& \
    >::type \
    { \
        lhs = static_cast< L >( lhs ) OPERAND rhs; \
        return lhs; \
    }
    
    #define DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_UNIT( OPERAND ) \
    template< \
        typename Left_Traits, \
        typename L, \
        typename Left_Scale, \
        typename Right_Traits, \
        typename R, \
        typename Right_Scale \
    > constexpr auto operator OPERAND##=( \
              unit<  Left_Traits, L,  Left_Scale >& lhs, \
        const unit< Right_Traits, R, Right_Scale >& rhs \
    ) -> typename std::enable_if< \
        conversion_exists< Left_Traits, L, Right_Traits, R >::value, \
        decltype( lhs )& \
    >::type \
    { \
        lhs = ( \
            static_cast< L >( lhs ) \
            OPERAND \
            static_cast< R >( unit< Left_Traits, R, Left_Scale >{ rhs } ) \
        ); \
        return lhs; \
    }
    
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_NONUNIT( + )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_NONUNIT( - )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_NONUNIT( * )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_NONUNIT( / )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_NONUNIT( % )
    // DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_NONUNIT( & ) No bitwise operators
    // DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_NONUNIT( | ) No bitwise operators
    // DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_NONUNIT( ^ ) No bitwise operators
    
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_UNIT( + )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_UNIT( - )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_UNIT( * )
    DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_UNIT( / )
    // DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_UNIT( % ) No way to preserve units
    // DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_UNIT( & ) No bitwise operators
    // DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_UNIT( | ) No bitwise operators
    // DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_UNIT( ^ ) No bitwise operators
    
    #undef DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_NONUNIT
    #undef DEFINE_OPERATORS_FOR_ASSIGNMENT_OPERAND_UNIT
} }


namespace JadeMatrix { namespace units // Unary operators //////////////////////
{
    #define DEFINE_OPERATORS_FOR_UNARY_OPERAND( OPERAND ) \
    template< \
        typename Traits, \
        typename T, \
        typename Scale \
    > \
    constexpr auto operator OPERAND( const unit< Traits, T, Scale >& rhs ) \
        -> unit< Traits, decltype( OPERAND static_cast< T >( rhs ) ), Scale > \
    { \
        return OPERAND static_cast< T >( rhs ); \
    }
    
    DEFINE_OPERATORS_FOR_UNARY_OPERAND( + )
    DEFINE_OPERATORS_FOR_UNARY_OPERAND( - )
    // DEFINE_OPERATORS_FOR_UNARY_OPERAND( ~ ) No bitwise operators
    
    #undef DEFINE_OPERATORS_FOR_UNARY_OPERAND
} }


#endif
