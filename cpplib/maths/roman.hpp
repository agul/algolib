#pragma once
#include <string>
#include <vector>

template<typename T>
std::string to_roman(const T number) {
    static const std::vector<uint32_t> values = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
    static const std::vector<std::string> digits = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};

    std::string res;
    size_t x = values.size() - 1;
    T n = number;
    while (n > 0) {
        while (values[x] > n) {
            --x;
        }
        n -= values[x];
        res += digits[x];
    }
    return res;
}
