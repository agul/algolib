#pragma once
#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

#include "string_view.hpp"

template<typename size_type = std::size_t>
std::vector<size_type> count_odd_length_palindromes(const StringView& string) {
    std::vector<size_type> odd(string.length(), 0);
    int32_t l = 0;
    int32_t r = -1;
    int32_t k;
    for (int32_t i = 0; i < string.length(); ++i) {
        if (i > r) {
            k = 1;
        } else {
            k = std::min<int32_t>(odd[l + r - i], r - i);
        }

        while (0 <= i - k && i + k < string.length() && string[i - k] == string[i + k]) {
            ++k;
        }
        odd[i] = k;
        if (i + k - 1 > r) {
            r = i + k - 1;
            l = i - k + 1;
        }
    }
    return odd;
}

template<typename size_type = std::size_t>
std::vector<size_type> count_even_length_palindromes(const StringView& string) {
    std::vector<size_type> even(string.length(), 0);
    int32_t l = 0;
    int32_t r = -1;
    int32_t k;
    for (int32_t i = 0; i < string.length(); ++i) {
        if (i > r) {
            k = 0;
        } else {
            k = std::min<int32_t>(even[l + r - i + 1], r - i + 1);
        }

        while (i + k < string.length() && i - k - 1 >= 0 && string[i + k] == string[i - k - 1]) {
            k++;
        }
        even[i] = k;

        if (i + k - 1 > r) {
            l = i - k;
            r = i + k - 1;
        }
    }
    return even;
}

template<typename size_type = std::size_t>
std::vector<size_type> count_palindromes(const StringView& string) {
    std::vector<size_type> result(string.length(), 0);
    const auto odd = count_odd_length_palindromes(string);
    const auto even = count_even_length_palindromes(string);
    std::transform(odd.cbegin(), odd.cend(), even.cbegin(), result.begin(), std::plus<size_type>());
    return result;
}
