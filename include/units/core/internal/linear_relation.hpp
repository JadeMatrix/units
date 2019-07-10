#pragma once
#ifndef JM_UNITS_CORE_INTERNAL_LINEAR_RELATION_HPP
#define JM_UNITS_CORE_INTERNAL_LINEAR_RELATION_HPP


#include "core_type_detection.hpp"
#include "core_types.hpp"
#include "utils.hpp"    // void_t, remove_cvref_t

#include <utility>      // declval
#include <type_traits>  // true_type, false_type


namespace JadeMatrix { namespace units { namespace internal // Constants lookup
{
    // Default values & types of numerator, denominator, and intercept if they
    // don't exist in the given relationship
    
    #define DEFINE_DEFAULT_RELATION_FIELD( FIELD, DEFAULT ) \
    template< \
        typename Relation, \
        typename T, \
        typename = void \
    > struct FIELD##_of \
    { \
        static constexpr unsigned char value = DEFAULT; \
    }; \
    template< typename Relation, typename T > struct FIELD##_of< \
        Relation, \
        T, \
        void_t< decltype( Relation::template values< T >::FIELD ) > \
    > \
    { \
        static constexpr auto value = Relation::template values< T >::FIELD; \
    };
    
    DEFINE_DEFAULT_RELATION_FIELD( slope_num, 1 )
    DEFINE_DEFAULT_RELATION_FIELD( slope_den, 1 )
    DEFINE_DEFAULT_RELATION_FIELD( intercept, 0 )
    
    #undef DEFINE_DEFAULT_RELATION_FIELD
} } }


namespace JadeMatrix { namespace units { namespace internal // One-way /////////
{
    template<
        typename YTraits,
        typename XTraits,
        typename = void
    > struct one_way_linear_relation;
    
    template<
        typename YTraits,
        typename XTraits
    > struct one_way_linear_relation<
        YTraits,
        XTraits,
        // ADL lookup must be duplicated here to take part in SFINAE
        void_t< decltype( units_linear_relation_lookup(
            std::declval< YTraits >(),
            std::declval< XTraits >()
        ) ) >
    >
    {
        using exists_type = void;
        
        // ADL lookup for linear relation type
        using _lookup = decltype( units_linear_relation_lookup(
            std::declval< YTraits >(),
            std::declval< XTraits >()
        ) );
        template< typename T > struct _values
        {
            static constexpr auto slope_num = slope_num_of< _lookup, T >::value;
            static constexpr auto slope_den = slope_den_of< _lookup, T >::value;
            static constexpr auto intercept = intercept_of< _lookup, T >::value;
        };
        
        // In most cases this will just be T
        template< typename T > using _common_type = typename std::common_type<
            remove_cvref_t< T >,
            decltype( _values< T >::slope_num ),
            decltype( _values< T >::slope_den ),
            decltype( _values< T >::intercept )
        >::type;
        
        template< typename T > static constexpr _common_type< T > apply( T&& v )
        {
            using ct = _common_type< T >;
            return (
                  static_cast< ct >( v )
                * static_cast< ct >( _values< T >::slope_num )
                / static_cast< ct >( _values< T >::slope_den )
                + static_cast< ct >( _values< T >::intercept )
            );
        }
    };
    
    template<
        typename YTraits,
        typename XTraits,
        typename = void
    > struct one_way_linear_relation_exists : std::false_type {};
    template<
        typename YTraits,
        typename XTraits
    > struct one_way_linear_relation_exists<
        YTraits,
        XTraits,
        typename one_way_linear_relation< YTraits, XTraits >::exists_type
    > : std::true_type {};
} } }


namespace JadeMatrix { namespace units { namespace internal // Two-way /////////
{
    template<
        typename YTraits,
        typename XTraits,
        typename = void
    > struct linear_relation;
    
    template<
        typename YTraits,
        typename XTraits
    > struct linear_relation<
        YTraits,
        XTraits,
        typename std::enable_if< one_way_linear_relation_exists<
            YTraits,
            XTraits
        >::value >::type
    > : one_way_linear_relation< YTraits, XTraits > {};
    
    template<
        typename YTraits,
        typename XTraits
    > struct linear_relation<
        YTraits,
        XTraits,
        typename std::enable_if<
            // If there is an explicit primary relation, prefer that
            !one_way_linear_relation_exists<
                YTraits,
                XTraits
            >::value
            && one_way_linear_relation_exists<
                XTraits,
                YTraits
            >::value
        >::type
    >
    {
        using exists_type = void;
        
        using _inverse = one_way_linear_relation< XTraits, YTraits >;
        
        template<
            typename T
        > using _common_type = typename _inverse::template _common_type< T >;
        template<
            typename T
        > using _values = typename _inverse::template _values< T >;
        
        template< typename T > static constexpr _common_type< T > apply( T&& v )
        {
            using ct = _common_type< T >;
            return (
                (
                      static_cast< ct >( v )
                    - static_cast< ct >( _values< T >::intercept )
                )
                * static_cast< ct >( _values< T >::slope_den )
                / static_cast< ct >( _values< T >::slope_num )
            );
        }
    };
    
    template<
        typename YTraits,
        typename XTraits,
        typename = void
    > struct linear_relation_exists : std::false_type {};
    template<
        typename YTraits,
        typename XTraits
    > struct linear_relation_exists<
        YTraits,
        XTraits,
        typename linear_relation< YTraits, XTraits >::exists_type
    > : std::true_type {};
} } }


#endif
