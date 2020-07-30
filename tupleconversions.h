#ifndef TUPLECONVERSIONS_H
#define TUPLECONVERSIONS_H

#include <type_traits>
#include <tuple>
#include <assert.h>

#include "private/structsize.h"
#include "typelist.h"

/**
 * @file
 * @brief Contains functions that convert needed struct to std::tuple
 */

namespace TupleConversions {

namespace Detail {

template <typename>
struct IsTupleImpl : std::false_type{};
template <typename... Args>
struct IsTupleImpl<std::tuple<Args...>> : std::true_type{};

} // namespace TupleConversions::Detail

template <typename T>
struct IsTuple : Detail::IsTupleImpl<std::remove_reference_t<T>>{};
template <typename T>
inline constexpr bool IsTupleV = IsTuple<T>::value;

//template <typename T, template <typename...> class Template>
//struct isSpecializationOf : std::false_type {};
//template <template <typename...> class Template, typename... Args>
//struct isSpecializationOf<Template<Args...>, Template> : std::true_type {};

//template <typename T>
//using IsTuple = isSpecializationOf<T, std::tuple>;

template <typename Tuple>
struct TupleExtractor;

template <typename... T>
struct TupleExtractor<std::tuple<T...>>
{
    using List = Conversions::TypeList<T...>;
};

template <typename T/*, typename... Args*/>
constexpr auto makeTuple(T&& t) {
    // return std::make_tuple(HERE);
    //return to_tuple_impl(t, to_tuple_size<T>{});
    return /*TupleConversions::*/Detail::toTupleImpl(
                std::forward<T>(t),
                StructConversions::Detail::to_tuple_size<std::decay_t<T>>{}
                );
}



} // namespace TupleConversions

#endif // TUPLECONVERSIONS_H
