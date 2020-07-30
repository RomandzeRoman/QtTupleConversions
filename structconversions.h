#ifndef STRUCTCONVERSIONS_H
#define STRUCTCONVERSIONS_H

#include <tuple>
#include <type_traits>
#include <utility>

//#include "structuredbindings.h"
#include "tupleconversions.h"
#include "typelist.h"

/**
 * @file
 * @brief Contains functions that convert std::tuple to needed struct
 */

namespace StructConversions {

/**
 * @brief Contains struct parameters, used in converting T to std::tuple
 *
 * Contains parameters of struct T type, used in converting T to std::tuple
 * @param TypleType contains std::tuple, which returns when converts from T struct
 * @param size shows number of variables in T
 * @param structSize contains std::integral_constant<std::size_t, size>
 */
template <typename T,
          typename = std::enable_if_t<
              !TupleConversions::IsTuple<T>::value
              >
          >
struct StructExtractor {
    using TupleType = std::decay_t<
        decltype(
            TupleConversions::makeTuple(
                std::declval<T>()
                )
            )
        >;
    using ArgumentsList = typename TupleConversions::TupleExtractor<TupleType>::List;
    static constexpr size_t size = StructConversions::Detail::to_tuple_size<std::decay_t<T>>::value;
    static constexpr auto structSize = StructConversions::Detail::to_tuple_size<std::decay_t<T>>{};
};


namespace Detail {

template <typename T, typename Tuple, size_t... Is>
constexpr T makeFromTupleImpl(
        Tuple&& tuple, std::index_sequence<Is...>
        )
{
    return T{std::get<Is>(std::forward<Tuple>(tuple))...};
}

struct DoIsNaryConstructibleImpl
{
    template<typename T,
           typename... Args,
           typename = decltype(T{std::declval<Args>()...})
           >
    static std::true_type test(int);

    template<typename, typename...>
    static std::false_type test(...);
};

template<typename T, typename... Args>
struct IsNaryConstructibleImpl
    : DoIsNaryConstructibleImpl
{
    using type = decltype(test<T, Args...>(0));
};

template <typename T, typename... Args>
struct IsNaryConstructible
    : IsNaryConstructibleImpl<T, Args...>::type
{
    static_assert(
            sizeof...(Args) > 0,
            "Only useful for > 0 arguments"
            );
};

template <typename T, typename... Args>
struct IsBracesConstructibleImpl
    : IsNaryConstructible<T, Args...>
{};

//template <typename T, typename Arg>
//struct IsBracesConstructibleImpl
//    : IsDirectConstructible<T, Arg>
//{};

//struct DoIsBracesStructlyConstructible {
//    template<typename T,
//             typename... Args,
//             typename = std::enable_if_t<N == sizeof...(Args)>
//             >
//    static std::true_type test(int);

//    template<typename, typename...>
//    static std::false_type test(...);
//};

struct CheckArgumentsLength {
    template <size_t N,
              typename... Args
              >
    static std::enable_if_t<N == sizeof...(Args), std::true_type>
    test(int);

    template<size_t, typename...>
    static std::false_type test(...);

};

template <typename T,
          size_t N,
          typename... Args//,
          //typename = typename std::enable_if_t<N == sizeof...(Args)>
          >
struct IsBracesStrictlyConstructibleImpl
    //: IsBracesConstructibleImpl<T, Args...>
    //: DoIsBracesStructlyConstructible
{
private:
    static constexpr bool _constructible
        = std::conjunction_v<
            //decltype(DoIsBracesStructlyConstructible::test<T, Args...>(0)),
            IsBracesConstructibleImpl<T, Args...>,
            decltype(CheckArgumentsLength::test<N, Args...>(0))
            >;
public:
    using type = std::conditional_t<_constructible, std::true_type, std::false_type>;
};

} // namespace Detail

template <typename T, typename... Args/*, typename = decltype(T{declval<Args>()...})*/>
struct IsBracesStrictlyConstructible
    : Detail::IsBracesStrictlyConstructibleImpl<T, StructExtractor<T>::size, Args...>::type  //std::true_type
{};
template <typename T, /*size_t N,*/ typename... Args>
inline constexpr bool IsBracesStrictlyConstructibleV = IsBracesStrictlyConstructible<T, Args...>::value;


template <typename T, /*size_t N,*/ typename... Args/*, typename = decltype(T{declval<Args>()...})*/>
struct IsBracesConstructible
    : Detail::IsBracesConstructibleImpl<T, Args...>  //std::true_type
{};
//template <typename T, size_t N, typename...>
//struct isBracesConstructible : std::false_type {};
template <typename T, /*size_t N,*/ typename... Args>
inline constexpr bool IsBracesConstructibleV = IsBracesConstructible<T, Args...>::value;

// [DONE]TODO check if Tuple is std::tuple
// [DONE]TODO rename without using braces
template <typename T,
          typename Tuple,
          typename = std::enable_if_t<
              TupleConversions::IsTupleV<Tuple>
              >
          >
[[nodiscard]]
constexpr T makeFromTuple(Tuple&& tuple) {
    return Detail::makeFromTupleImpl<T>(
                std::forward<Tuple>(tuple),
                std::make_index_sequence<
                    std::tuple_size_v< std::decay_t<Tuple> >
                    >{}
                );
}











//template <typename T>
//struct TupleMaker {
//    constexpr auto operator()(T&& t) {
//        return StructConversions::makeTuple(std::forward<T>(t));
//    }
//    constexpr auto makeTuple(T&& t) {
//        return StructConversions::makeTuple(std::forward<T>(t));
//    }
//};




} // namespace StructConversions


#endif // STRUCTCONVERSIONS_H
