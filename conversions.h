#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <tuple>
#include <type_traits>
#include <utility>

#include <QDateTime>

/**
 * @file
 * @brief Functions used in common type conversions
 * @details Contains fuctions that used in type conversions -
 * std::tuple <-> struct <-> Json
 */

namespace Conversions {

template <typename T>
struct alwaysFalse : std::false_type{};

/**
 * @brief Create type name string from type
 *
 * @details Generate string, that contains name of type.
 * Support next types:
 * - int;
 * - double;
 * - qint64;
 * - bool;
 * - QString;
 * - QDateTime.
 *
 * All this types need to be stored in Json & SQL database
 * @return QString with name of type
 */
template <typename T>
constexpr QString typeName() {
    if constexpr (std::is_same<T, int>{}) {
        return "Integer";
    }
    else if constexpr (std::is_same<T, double>{}) {
        return "Double";
    }
    else if constexpr (std::is_same<T, qint64>{}) {
        return "Int64";
    }
    else if constexpr (std::is_same<T, bool>{}) {
        return "Bool";
    }/*
    else if constexpr (std::is_same<T, char>{}) {
        return "Char";
    }*/
    else if constexpr (std::is_same<T, QString>{}) {
        return "QString";
    }
    else if constexpr (std::is_same<T, QDateTime>{}) {
        return "QDateTime";
    }
    else {
        // TODO static_assert
        return "Unknown";
    }
}

/**
 * @brief Convert value of Type to value of stored or json`s type
 *
 * Convert Type to type, which can be stored in Json & SQL database
 * @param Type value needed to be converted
 * @return value of stored or json`s type
 */
template <typename Type>
constexpr auto toStoredDataValue(Type&& val) {
    using T = std::decay_t<Type>;
    if constexpr (std::is_same<T, bool>{}) {
        return val;
    }
    else if constexpr (std::is_same<T, int>{}) {
        return val;
    }
    else if constexpr (std::is_same<T, qint64>{}) {
        return val;
    }
    else if constexpr (std::is_same<T, double>{}) {
        return val;
    }/*
    else if constexpr (std::is_same<T, char>{}) {
        return QString(val);
    }*/
    else if constexpr (std::is_same<T, QString>{}) {
        return val;
    }
    else if constexpr (std::is_same<T, QDateTime>{}) {
        return val.toMSecsSinceEpoch();
    }
    else {
        //std::cout << val;
        return alwaysFalse<T>{};
    }
}


template <typename Type>
constexpr QString toStoredDataValueString(Type&& val) {
    using T = std::decay_t<Type>;
    if constexpr (std::is_same<T, bool>{}) {
        return QString::number(int(val));
    }
    else if constexpr (std::is_same<T, int>{}) {
        return QString::number(val);
    }
    else if constexpr (std::is_same<T, qint64>{}) {
        return QString::number(val);
    }
    else if constexpr (std::is_same<T, double>{}) {
        return QString::number(val);
    }/*
    else if constexpr (std::is_same<T, char>{}) {
        return QString(val);
    }*/
    else if constexpr (std::is_same<T, QString>{}) {
        return val;
    }
    else if constexpr (std::is_same<T, QDateTime>{}) {
        return QString::number(val.toMSecsSinceEpoch());
    }
    else {
        //std::cout << val;
        return alwaysFalse<T>{};
    }
}

/**
 * @brief Convert QVariant of stored or json`s type to value of Type
 *
 * Convert QVariant if value which can be stored in Json & SQL database
 * to value of Type
 * @param Type target value type needed to be converted to
 * @param var QVariant needed to be converted from - value of stored or json`s type
 * @return value of Type
 */
template <typename Type>
constexpr auto fromStoredVariant(const QVariant& var) {
    if constexpr (std::is_same<Type, bool>{}) {
        return var.toBool();
    }
    else if constexpr (std::is_same<Type, int>{}) {
        return var.toInt();
    }
    else if constexpr (std::is_same<Type, qint64>{}) {
        return var.toLongLong();
    }
    else if constexpr (std::is_same<Type, double>{}) {
        return var.toDouble();
    }
    else if constexpr (std::is_same<Type, QString>{}) {
        return var.toString();
    }
    else if constexpr (std::is_same<Type, QDateTime>{}) {
        return QDateTime::fromMSecsSinceEpoch(var.toLongLong());
    }
    else {
        //std::cout << val;
        return Conversions::alwaysFalse<Type>{};
    }

}


/**
 * @brief Checks if T is const lvalue
 * Contains true in value if T is constant lvalue reference
 */
template <typename T>
struct IsConstLvalue {
    static constexpr bool value = std::conjunction_v<
            std::is_lvalue_reference<T>,
            std::is_const<std::remove_reference_t<T>>
            >;
};

/**
 * @brief Checks if T is const lvalue
 * Shortcut to IsConstLvalue struct value variable
 * Contains true if T is constant lvalue reference
 */
template <typename T>
inline constexpr bool IsConstLvalueV = IsConstLvalue<T>::value;

} // namespace Conversions




#endif // CONVERSIONS_H
