#pragma once
#include <type_traits>

#include "maths.hpp"
#include "is_equal_to_zero.hpp"

// todo[c++17] use is_floating_point_v
template<typename T, typename std::enable_if_t<std::is_floating_point<T>::value>* = nullptr>
size_t solve_quadratic_equation(const T a, const T b, const T c, T& x1, T& x2) {
    const T D = sqr(b) - 4 * a * c;
    if (D < -EPS) {
        return 0;
    }
    if (is_equal_to_zero(D)) {
        const T x = -b / (a * 2);
        x1 = x;
        return 1;
    }
    x1 = (-b + std::sqrt(D)) / (a * 2);
    x2 = (-b - std::sqrt(D)) / (a * 2);
    return 2;
}
