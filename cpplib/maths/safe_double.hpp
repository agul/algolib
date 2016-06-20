#pragma once
#include <algorithm>
#include <cmath>

#include "base/constants.hpp"

namespace safe {

template<typename T>
constexpr T abs(const T arg) {
	return arg < 0 ? -arg : arg;
}

template<typename T>
constexpr bool is_zero(const T arg) {
	return abs(arg) < EPS;
}

template<typename T>
constexpr bool equal(const T lhs, const T rhs) {
	return is_zero(lhs - rhs);
}

template<typename T>
constexpr bool not_equal(const T lhs, const T rhs) {
	return !equal(lhs, rhs);
}

template<typename T>
constexpr bool less_or_equal(const T lhs, const T rhs) {
	return lhs < rhs || equal(lhs, rhs);
}

template<typename T>
constexpr bool less(const T lhs, const T rhs) {
	return lhs < rhs && !equal(lhs, rhs);
}

template<typename T>
constexpr bool greater_or_equal(const T lhs, const T rhs) {
	return less_or_equal(rhs, lhs);
}

template<typename T>
constexpr bool greater(const T lhs, const T rhs) {
	return less(rhs, lhs);
}

template<typename T>
T sqrt(const T arg) {
	static_assert(std::is_floating_point<T>(), "Implemented only for floating point types.");
	if (is_zero(arg)) {
		return 0;
	}
	return std::sqrt(arg);
}

template<typename T>
T asin(const T arg) {
	static_assert(std::is_floating_point<T>(), "Implemented only for floating point types.");
	const T tmp_arg = std::min(static_cast<T>(1.0), std::max(static_cast<T>(-1.0), arg));
	return std::asin(tmp_arg);
}

template<typename T>
T acos(const T arg) {
	static_assert(std::is_floating_point<T>(), "Implemented only for floating point types.");
	const T tmp_arg = std::min(static_cast<T>(1.0), std::max(static_cast<T>(-1.0), arg));
	return std::acos(tmp_arg);
}

}