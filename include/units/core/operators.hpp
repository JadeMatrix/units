#pragma once
#ifndef JM_UNITS_CORE_OPERATORS_HPP
#define JM_UNITS_CORE_OPERATORS_HPP


#include "internal/conversion.hpp"
#include "internal/core_type_detection.hpp"
#include "internal/core_types.hpp"
#include "internal/utils.hpp"   // remove_cvref_t
#include "internal/reduce.hpp"

#include <type_traits>          // enable_if, is_convertible
#include <utility>              // declval


namespace JadeMatrix { namespace units // Multiplication & division ////////////
{
    // This used to be elegant, but also incorrect, so welcome to this mess
    #define DEFINE_UNIT_OPERATORS_MULTDIV( OPERAND, COMBINE ) \
    namespace internal \
    { \
        template< \
            typename LHS, \
            typename RHS, \
            typename = void \
        > struct operator_dispatch_to_##COMBINE; \
         \
        template< \
            typename LHS, \
            typename RHS \
        > struct operator_dispatch_to_##COMBINE< \
            LHS, \
            RHS, \
            typename std::enable_if< ( \
                   internal::is_unit< LHS >::value \
                && internal::is_unit< RHS >::value \
            ) >::type \
        > \
        { \
            static constexpr auto value( \
                const LHS& lhs, \
                const RHS& rhs \
            ) -> internal::reduced< COMBINE< \
                LHS::template unit_type, \
                RHS::template unit_type, \
                internal::remove_cvref_t< decltype( \
                    static_cast< typename LHS::value_type >( lhs ) \
                    OPERAND \
                    static_cast< typename RHS::value_type >( rhs ) \
                ) > \
            > > \
            { \
                return ( \
                    static_cast< typename LHS::value_type >( lhs ) \
                    OPERAND \
                    static_cast< typename RHS::value_type >( rhs ) \
                ); \
            } \
        }; \
         \
        template< \
            typename LHS, \
            typename RHS \
        > struct operator_dispatch_to_##COMBINE< \
            LHS, \
            RHS, \
            typename std::enable_if< ( \
                    internal::is_unit< LHS >::value \
                && !internal::is_unit< RHS >::value \
            ) >::type \
        > \
        { \
            static constexpr auto value( \
                const LHS& lhs, \
                const RHS& rhs \
            ) -> decltype( operator_dispatch_to_##COMBINE< \
                LHS, \
                ratio< RHS > \
            >::value( lhs, rhs ) ) \
            { \
                return operator_dispatch_to_##COMBINE< \
                    LHS, \
                    ratio< RHS > \
                >::value( lhs, rhs ); \
            } \
        }; \
         \
        template< \
            typename LHS, \
            typename RHS \
        > struct operator_dispatch_to_##COMBINE< \
            LHS, \
            RHS, \
            typename std::enable_if< ( \
                   !internal::is_unit< LHS >::value \
                &&  internal::is_unit< RHS >::value \
            ) >::type \
        > \
        { \
            static constexpr auto value( \
                const LHS& lhs, \
                const RHS& rhs \
            ) -> decltype( operator_dispatch_to_##COMBINE< \
                ratio< LHS >, \
                RHS \
            >::value( lhs, rhs ) ) \
            { \
                return operator_dispatch_to_##COMBINE< \
                    ratio< LHS >, \
                    RHS \
                >::value( lhs, rhs ); \
            } \
        }; \
    } \
     \
    template< \
        typename LHS, \
        typename RHS \
    > constexpr auto operator OPERAND( \
        const LHS& lhs, \
        const RHS& rhs \
    ) -> decltype( internal::operator_dispatch_to_##COMBINE< \
        LHS, \
        RHS \
    >::value( lhs, rhs ) ) \
    { \
        return internal::operator_dispatch_to_##COMBINE< \
            LHS, \
            RHS \
        >::value( lhs, rhs ); \
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
    ) -> typename std::enable_if<
        (
               internal::is_unit< LHS >::value
            && internal::is_unit< RHS >::value
            && std::is_convertible< LHS, RHS >::value
        ),
        typename LHS::template unit_type< internal::remove_cvref_t< decltype(
              static_cast< typename LHS::value_type >( lhs )
            % static_cast< typename RHS::value_type >( rhs )
        ) > >
    >::type
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
    #define DEFINE_UNIT_OPERATORS_ADDSUB( OPERAND ) \
    template< \
        typename LHS, \
        typename RHS \
    > constexpr auto operator OPERAND( \
        const LHS& lhs, \
        const RHS& rhs \
    ) -> typename std::enable_if< \
        ( \
               internal::is_unit< LHS >::value \
            && internal::is_unit< RHS >::value \
            && std::is_convertible< LHS, RHS >::value \
        ), \
        typename LHS::template unit_type< \
            internal::remove_cvref_t< decltype( \
                static_cast< typename LHS::value_type >( lhs ) \
                OPERAND \
                static_cast< typename RHS::value_type >( rhs ) \
            ) > \
        > \
    >::type \
    { \
        return ( \
            static_cast< typename LHS::value_type >( lhs ) \
            OPERAND \
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
    #define DEFINE_UNIT_OPERATORS_COMPARISON( OPERAND ) \
    template< \
        typename LHS, \
        typename RHS \
    > constexpr auto operator OPERAND( \
        const LHS& lhs, \
        const RHS& rhs \
    ) -> typename std::enable_if< \
        ( \
               internal::is_unit< LHS >::value \
            && internal::is_unit< RHS >::value \
            && std::is_convertible< LHS, RHS >::value \
        ), \
        internal::remove_cvref_t< decltype( \
            static_cast< typename LHS::value_type >( lhs ) \
            OPERAND \
            static_cast< typename RHS::value_type >( rhs ) \
        ) > \
    >::type \
    { \
        return ( \
            static_cast< typename LHS::value_type >( lhs ) \
            OPERAND \
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
    // TODO: `internal::is_unit< RHS >::value` for stronger type safety
    #define DEFINE_UNIT_OPERATORS_ASSIGN( OPERAND ) \
    template< \
        typename LHS, \
        typename RHS \
    > constexpr auto operator OPERAND##=( \
              LHS& lhs, \
        const RHS& rhs \
    ) -> typename std::enable_if< \
        ( \
            internal::is_unit< LHS >::value \
            && std::is_convertible< LHS, RHS >::value \
        ), \
        LHS& \
    >::type \
    { \
        return ( lhs = ( lhs OPERAND rhs ), lhs ); \
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
