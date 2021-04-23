#pragma once
#include <cstdlib>
#include <type_traits>

#include "base/constants.hpp"

template<typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline bool is_equal_to_zero(const T& a) {
    return abs(a) < EPS;
}

template<typename T, typename std::enable_if<!std::is_floating_point<T>::value>::type* = nullptr>
inline bool is_equal_to_zero(const T& a) {
    return a == 0;
}
