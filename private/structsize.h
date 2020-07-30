#ifndef STRUCTSIZE_H
#define STRUCTSIZE_H

#include <tuple>
#include <type_traits>
//#include <cassert>


//#include "structuredbindings_generated.h"

#include "structurebindings_generated.h"

/**
 * @file
 * @brief Contains detail functions that detect size of struct
 *
 * Contains detail functions that detect number of variables contained
 * in struct.
 * @todo Rename functions to correspond codestyle
 * @warning Dont use this functions directly!
 */

namespace StructConversions::Detail {

/**
 * @brief Struct that can cover any type
 *
 * Struct that can cover any type, used when check
 * number of arguments in constructor
 * @todo FIXME change to comments below
 * @warning Dont use this struct!
 */
struct AnyType {
    template <typename T>
    constexpr operator T();
};

// FIXME change to below
//template <typename TParent>
//struct AnyType {
//    template <
//            typename T,
//            typename = std::enable_if_t<
//                !std::is_same<TParent, T>{}
//                >
//            >
//    constexpr operator T();
//};

/**
 * @brief Checks if struct constructible with I elements
 *
 * Checks if struct can be aggregate initialized
 * (braces constructible) with I elements
 * @warning Dont use this struct!
 * @todo TODO maybe move to private section of some struct
 * @todo TODO rename
 */
template<class T, std::size_t... I>
decltype(void(T{(I, std::declval<AnyType>())...}), std::true_type{})
test_is_braces_constructible_n(std::index_sequence<I...>);

/**
 * @brief Instantiates when struct NOT constructible with I elements
 *
 * Instantiates if struct can NOT be aggregate initialized
 * (braces constructible) with I elements
 */
template <class, class...>
std::false_type test_is_braces_constructible_n(...);

/**
 * @brief Convinience typedef to checkif struct aggregate constructible
 *
 * Typedef used to check if struct aggregate constructible with I elements
 * @warning Dont use this struct!
 * @todo TODO maybe move to private section of some struct
 * @todo TODO rename
 */
template <class T, std::size_t N>
using is_braces_constructible_n =
    decltype(test_is_braces_constructible_n<T>(std::make_index_sequence<N>{}));

/**
 * @brief Shows size of struct
 *
 * Function detects number of variables in struct
 * by binary search in aggregate initializer
 * (braces initializer)
 * @todo TODO rename
 * @todo TODO maybe move to private section of some struct
 * @warning Dont use this function directly!
 */
template<class T, std::size_t L = 0u, std::size_t R = sizeof(T) + 1u>
constexpr std::size_t to_tuple_size_f() {
    constexpr std::size_t M = (L + R) / 2u;
    if constexpr (M == 0) {
        //static_assert(R==1);
        static_assert(std::is_empty<T>{});
        return 0u;
        //return std::is_empty<T>{} ? 0u : throw "Unable to determine number of elements";
    }
    else if constexpr (L == M) {
        return M;
    }
    else if constexpr (is_braces_constructible_n<T, M>{}) {
        return to_tuple_size_f<T, M, R>();
    }
    else {
        return to_tuple_size_f<T, L, M>();
    }
}


/**
 * @brief Show size of struct
 *
 * Typedef shows number of variables in struct
 * @todo TODO rename
 * @warning Dont use this typedef!
 * @todo TODO maybe move to private section of some struct
 */
template<typename T>
struct to_tuple_size : std::integral_constant<
        std::size_t, to_tuple_size_f<T>()
>{};

} // StructConversions::Detail

//template<class T, class = struct current_value, std::size_t = TO_TUPLE_MAX, class = struct required_value, std::size_t N>
//auto to_tuple_impl(T&&, std::integral_constant<std::size_t, N>) noexcept {
//    static_assert(N <= TO_TUPLE_MAX, "Please increase TO_TUPLE_MAX");
//}

//template<class T, class = std::enable_if_t<std::is_class<T>::value && std::is_standard_layout<T>::value>>
//auto to_tuple(T&& object) noexcept {
//    return to_tuple_impl(std::forward<T>(object), to_tuple_size<std::decay_t<T>>{});
//}

#endif // STRUCTSIZE_H
