#pragma once
#ifndef JM_UNITS_CORE_OPERATORS_HPP
#define JM_UNITS_CORE_OPERATORS_HPP


#include "internal/conversion.hpp"
#include "internal/core_type_detection.hpp"
#include "internal/core_types.hpp"
#include "internal/utils.hpp"   // remove_cvref_t
#include "internal/reduce.hpp"

#include <type_traits>          // enable_if, is_constructible
#include <utility>              // declval


namespace JadeMatrix { namespace units // Multiplication & division ////////////
{
    #define DEFINE_UNIT_OPERATORS_MULTDIV( OPERATOR, COMBINE ) \
    template< \
        typename LHS, \
        typename RHS \
    > constexpr auto operator OPERATOR( \
        const LHS& lhs, \
        const RHS& rhs \
    ) -> internal::reduced< COMBINE< \
            LHS::template unit_type, \
            RHS::template unit_type, \
            internal::remove_cvref_t< decltype( \
                static_cast< typename LHS::value_type >( lhs ) \
                OPERATOR \
                static_cast< typename RHS::value_type >( rhs ) \
            ) > \
        > \
    > \
    { \
        return ( \
            static_cast< typename LHS::value_type >( lhs ) \
            OPERATOR \
            static_cast< typename RHS::value_type >( rhs ) \
        ); \
    }
    
    DEFINE_UNIT_OPERATORS_MULTDIV( *, by  )
    DEFINE_UNIT_OPERATORS_MULTDIV( /, per )
    
    #undef DEFINE_UNIT_OPERATORS_MULTDIV
} }


namespace JadeMatrix { namespace units // Modulo ///////////////////////////////
{
    // Modulo returns the left-hand unit; it is being treated as the remainder
    // of left-hand units when divided into right-hand units.
    template<
        typename LHS,
        typename RHS
    > constexpr auto operator %(
        const LHS& lhs,
        const RHS& rhs
    ) -> typename LHS::template unit_type< decltype(
          static_cast< typename LHS::value_type >( lhs )
        % static_cast< typename RHS::value_type >( rhs )
    ) >
    {
        return (
              static_cast< typename LHS::value_type >( lhs )
            % static_cast< typename RHS::value_type >(
                static_cast<
                    typename LHS::template unit_type<
                        typename RHS::value_type
                    >
                >( rhs )
            )
        );
    }
} }


namespace JadeMatrix { namespace units // Addition & subtration ////////////////
{
    #define DEFINE_UNIT_OPERATORS_ADDSUB( OPERATOR ) \
    template< \
        typename LHS, \
        typename RHS \
    > constexpr auto operator OPERATOR( \
        const LHS& lhs, \
        const RHS& rhs \
    ) -> typename std::enable_if< ( \
        ( \
               internal::is_unit< LHS >::value \
            || internal::is_unit< RHS >::value \
        ) \
        && std::is_constructible< LHS, RHS >::value \
    ), typename LHS::template unit_type< \
        internal::remove_cvref_t< \
            decltype( \
                static_cast< typename LHS::value_type >( lhs ) \
                OPERATOR \
                static_cast< typename RHS::value_type >( rhs ) \
            ) \
        > \
    > >::type \
    { \
        return ( \
            static_cast< typename LHS::value_type >( lhs ) \
            OPERATOR \
            static_cast< typename RHS::value_type >( \
                static_cast< \
                    typename LHS::template unit_type< \
                        typename RHS::value_type \
                    > \
                >( rhs ) \
            ) \
        ); \
    }
    
    DEFINE_UNIT_OPERATORS_ADDSUB( + )
    DEFINE_UNIT_OPERATORS_ADDSUB( - )
    
    #undef DEFINE_UNIT_OPERATORS_ADDSUB
} }


namespace JadeMatrix { namespace units // Comparison ///////////////////////////
{
    #define DEFINE_UNIT_OPERATORS_COMPARISON( OPERATOR ) \
    template< \
        typename LHS, \
        typename RHS \
    > constexpr auto operator OPERATOR( \
        const LHS& lhs, \
        const RHS& rhs \
    ) -> typename std::enable_if< ( \
           internal::is_unit< LHS >::value \
        || internal::is_unit< RHS >::value \
    ), decltype( \
        static_cast< typename LHS::value_type >( lhs ) \
        OPERATOR \
        static_cast< typename RHS::value_type >( rhs ) \
    ) >::type \
    { \
        return ( \
            static_cast< typename LHS::value_type >( lhs ) \
            OPERATOR \
            static_cast< typename RHS::value_type >( \
                static_cast< \
                    typename LHS::template unit_type< \
                        typename RHS::value_type \
                    > \
                >( rhs ) \
            ) \
        ); \
    }
    
    DEFINE_UNIT_OPERATORS_COMPARISON( == )
    DEFINE_UNIT_OPERATORS_COMPARISON( != )
    DEFINE_UNIT_OPERATORS_COMPARISON( <  )
    DEFINE_UNIT_OPERATORS_COMPARISON( >  )
    DEFINE_UNIT_OPERATORS_COMPARISON( <= )
    DEFINE_UNIT_OPERATORS_COMPARISON( >= )
    
    #undef DEFINE_UNIT_OPERATORS_COMPARISON
} }


namespace JadeMatrix { namespace units // Assignment ///////////////////////////
{
    #define DEFINE_UNIT_OPERATORS_ASSIGN( OPERATOR ) \
    template< \
        typename LHS, \
        typename RHS \
    > constexpr auto operator OPERATOR##=( \
              LHS& lhs, \
        const RHS& rhs \
    ) -> typename std::enable_if< ( \
        internal::is_unit< LHS >::value \
        && std::is_constructible< LHS, RHS >::value \
    ), LHS& >::type \
    { \
        return ( lhs = ( lhs OPERATOR rhs ), lhs ); \
    }
    
    DEFINE_UNIT_OPERATORS_ASSIGN( + )
    DEFINE_UNIT_OPERATORS_ASSIGN( - )
    DEFINE_UNIT_OPERATORS_ASSIGN( * )
    DEFINE_UNIT_OPERATORS_ASSIGN( / )
    DEFINE_UNIT_OPERATORS_ASSIGN( % )
    
    #undef DEFINE_UNIT_OPERATORS_ASSIGN
} }


namespace JadeMatrix { namespace units // Unary ////////////////////////////////
{
    #define DEFINE_UNIT_OPERATORS_UNARY( OPERAND ) \
    template< typename RHS > \
    constexpr auto operator OPERAND( const RHS& rhs ) \
        -> typename RHS::template unit_type< decltype( \
            OPERAND static_cast< typename RHS::value_type >( rhs ) \
        ) > \
    { \
        return OPERAND static_cast< typename RHS::value_type >( rhs ); \
    }
    
    DEFINE_UNIT_OPERATORS_UNARY( + )
    DEFINE_UNIT_OPERATORS_UNARY( - )
    
    #undef DEFINE_UNIT_OPERATORS_UNARY
} }


#endif
