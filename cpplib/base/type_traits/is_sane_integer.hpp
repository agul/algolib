#pragma once
#include <type_traits>

#include "disjunction.hpp"

// todo[c++17] use is_same_v
// todo[c++17] use variable template
template <typename T, typename... Types>
using is_one_of = Disjunction<std::is_same<T, Types>::value...>;

// todo[c++17] use is_integral_v
// todo[c++17] use bool_constant
// todo[c++17] use variable template
template <typename T>
using is_sane_integer = std::integral_constant<
        bool,
        std::is_integral<T>::value && !is_one_of<typename std::remove_cv_t<T>, char, signed char, unsigned char, bool, char16_t, char32_t, wchar_t>::value
>;
