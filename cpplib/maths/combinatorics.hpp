#pragma once
#include <vector>

template<typename T>
T factorial(const size_t n) {
    T ret = 1;
    for (size_t i = 2; i <= n; ++i) {
        ret *= i;
    }
    return ret;
}

template<typename T>
void binomial_coefficients(const size_t n, std::vector<std::vector<T>>* c) {
    std::vector<std::vector<T>> result(n, std::vector<T>(n));
    for (size_t i = 0; i < n; ++i) {
        result[i][0] = 1;
        result[i][i] = 1;
        for (size_t j = 1; j < i; ++j) {
            result[i][j] = result[i - 1][j - 1] + result[i - 1][j];
        }
    }
    c->swap(result);
}

template<typename T>
std::vector<T> calc_powers(const T base, const size_t n) {
    std::vector<T> powers(n + 1);
    powers[0] = 1;
    for (size_t i = 1; i < powers.size(); ++i) {
        powers[i] = powers[i - 1] * base;
    }
    return powers;
}

template<typename T>
std::vector<T> calc_factorial(const size_t n) {
    std::vector<T> factorial(n + 1);
    factorial[0] = 1;
    for (size_t i = 1; i < factorial.size(); ++i) {
        factorial[i] = factorial[i - 1] * i;
    }
    return factorial;
}
