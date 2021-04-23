#pragma once
#include <cmath>
#include <vector>

#include "random.hpp"

template<typename T>
inline bool is_prime(const T& n)
// Straightforward check in O(sqrt(N))
{
    if (n < 2) {
        return false;
    }
    const T floored_square_root = static_cast<T>(sqrt(n));
    for (T i = 2; i <= floored_square_root; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

template<typename T>
inline T next_prime(const T& n) {
    for (T candidate = n; ; ++candidate) {
        if (is_prime(candidate)) {
            return candidate;
        }
    }
}

template<typename T>
inline T random_prime(const T from, const T to)
// note that resulting prime is not necessary from [from, to] interval
// [from, to] sets an interval to select a random integer to get next_prime()
{
    return next_prime(Random::get(from, to));
}

template<typename T = uint32_t>
inline T random_big_prime(const T from = 1000000000, const T to = INF) {
    return random_prime(from, to);
}

inline std::vector<bool> eratosthenes_sieve(const size_t n) {
    if (n < 2) {
        return std::vector<bool>(n, false);
    }
    std::vector<bool> prime(n, true);
    prime[0] = false;
    prime[1] = false;
    for (size_t i = 4; i < n; i += 2) {
        prime[i] = false;
    }
    for (size_t i = 3; i * i < n; i += 2) {
        if (prime[i]) {
            const size_t delta = i * 2;
            for (size_t j = i * i; j < n; j += delta) {
                prime[j] = false;
            }
        }
    }
    return prime;
}

template<typename T = uint32_t>
inline std::vector<T> primes_vector(const size_t n) {
    if (n < 2) {
        return {};
    }
    const std::vector<bool> prime = eratosthenes_sieve(n);
    std::vector<T> result = {2};
    for (size_t i = 3; i < n; i += 2) {
        if (prime[i]) {
            result.emplace_back(i);
        }
    }
    return result;
}

template<typename T = uint32_t>
inline std::vector<T> min_prime_div_vector(const size_t n) {
    std::vector<T> result(n, 0);
    result[0] = 1;
    result[1] = 1;
    for (size_t i = 2; i < n; i += 2) {
        result[i] = 2;
    }
    size_t index = 3;
    while (index * index < n) {
        if (result[index] == 0) {
            result[index] = index;
            const size_t delta = index * 2;
            for (size_t j = index * index; j < n; j += delta) {
                if (result[j] == 0) {
                    result[j] = index;
                }
            }
        }
        index += 2;
    }
    while (index < n) {
        if (result[index] == 0) {
            result[index] = index;
        }
        ++index;
    }
    return result;
}

template<typename T, typename size_type = std::size_t>
std::vector<std::pair<T, size_type>> get_prime_divisors_vector(const T n, const std::vector<T>& min_prime_div_vector) {
    std::vector<std::pair<T, size_type>> prime_divisors;
    T cur = n;
    while (cur > 1) {
        const T min_div = min_prime_div_vector[cur];
        if (!prime_divisors.empty() && prime_divisors.back().first == min_div) {
            ++prime_divisors.back().second;
        } else {
            prime_divisors.emplace_back(min_div, 1);
        }
        cur /= min_div;
    }
    return prime_divisors;
}

template<typename T, typename size_type = std::size_t>
std::vector<std::pair<T, size_type>> get_prime_divisors_vector(const T n) {
    std::vector<std::pair<T, size_type>> prime_divisors;
    T cur = n;
    for (T i = 2; static_cast<int64_t>(i) * i <= cur; ++i) {
        if (cur % i == 0) {
            size_type degree = 0;
            while (cur % i == 0) {
                ++degree;
                cur /= i;
            }
            prime_divisors.emplace_back(i, degree);
        }
    }
    if (cur > 1) {
        prime_divisors.emplace_back(cur, 1);
    }
    return prime_divisors;
}
