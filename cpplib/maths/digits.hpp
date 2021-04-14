#pragma once
#include <cstdlib>
#include <vector>

template<typename T>
T digit_sum(const T n) {
    T res = 0;
    T tmp = std::abs(n);
    while (tmp > 0) {
        res += tmp % 10;
        tmp /= 10;
    }
    return res;
}

template<typename T>
std::vector<size_t> digits(const T n) {
    if (n == 0) {
        return {0};
    }
    std::vector<size_t> res;
    T tmp = std::abs(n);
    while (tmp > 0) {
        res.emplace_back(tmp % 10);
        tmp /= 10;
    }
    return res;
}

template<typename T>
size_t digits_count(const T n) {
    return digits(n).size();
}
