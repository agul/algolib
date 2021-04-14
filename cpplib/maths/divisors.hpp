#pragma once
#include <algorithm>
#include <numeric>
#include <vector>

template<typename T>
std::vector<T> divisors_vector(const T n) {
    const auto approximate_divisors_count = static_cast<size_t>(std::cbrt(n));
    std::vector<T> ans;
    ans.reserve(approximate_divisors_count);
    const auto floored_square_root = static_cast<T>(std::sqrt(n));
    for (T i = 1; i <= floored_square_root; ++i) {
        if (n % i == 0) {
            ans.emplace_back(i);
            if (n / i != i) {
                ans.emplace_back(n / i);
            }
        }
    }
    return ans;
}

template<typename T, typename size_type = std::size_t>
std::vector<T> divisors_vector(const std::vector<std::pair<T, size_type>>& prime_divisors, const bool sorted = false) {
    static std::vector<T> buffer;

    size_t divisors_count = 1;
    for (const auto& prime_divisor : prime_divisors) {
        divisors_count *= prime_divisor.second + 1;
    }

    std::vector<T> factors = {1};
    factors.reserve(divisors_count);
    if (sorted) {
        buffer.resize(divisors_count);
    }

    for (const auto& prime_divisor : prime_divisors) {
        const T prime_div = prime_divisor.first;
        const size_t exponent = prime_divisor.second;

        const size_t prev_size = factors.size();
        for (size_t i = 0; i < exponent * prev_size; ++i) {
            factors.emplace_back(factors[factors.size() - prev_size] * prime_div);
        }

        if (sorted && factors[prev_size - 1] > prime_div) {
            for (size_t section = prev_size; section < factors.size(); section *= 2) {
                for (size_t i = 0; i + section < factors.size(); i += 2 * section) {
                    const size_t length = std::min(2 * section, factors.size() - i);
                    std::merge(factors.cbegin() + i,
                               factors.cbegin() + i + section,
                               factors.cbegin() + i + section,
                               factors.cbegin() + i + length,
                               buffer.begin());
                    std::copy(buffer.cbegin(), buffer.cbegin() + length, factors.begin() + i);
                }
            }
        }
    }
    return factors;
}
