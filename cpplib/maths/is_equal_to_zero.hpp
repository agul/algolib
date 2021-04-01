#pragma once
#include <cstdlib>
#include <type_traits>

#include "base/constants.hpp"
#include "maths/decimal.hpp"

template<typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline constexpr bool is_equal_to_zero(const T& a) {
	return std::abs(a) < EPS;
}

template<typename T, typename std::enable_if<!std::is_floating_point<T>::value>::type* = nullptr>
inline constexpr bool is_equal_to_zero(const T& a) {
	return a == 0;
}
