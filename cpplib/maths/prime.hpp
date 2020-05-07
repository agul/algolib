#pragma once
#include <cmath>
#include <vector>

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

inline void eratosthenes_sieve(const size_t n, std::vector<bool>* prime) {
	if (n < 2) {
		prime->assign(n, false);
		return;
	}
	std::vector<bool> result(n, true);
	result[0] = result[1] = false;
	for (size_t i = 4; i < n; i += 2) {
		result[i] = false;
	}
	for (size_t i = 3; i * i < n; i += 2) {
		if (result[i]) {
			const size_t delta = i << 1;
			for (size_t j = i * i; j < n; j += delta) {
				result[j] = false;
			}
		}
	}
	prime->swap(result);
}

inline void eratosthenes_sieve(std::vector<bool>* prime) {
	eratosthenes_sieve(prime->size(), prime);
}

template<typename T>
inline void primes_vector(const size_t n, std::vector<T>* primes) {
	std::vector<T> result;
	if (n < 2) {
		primes->swap(result);
		return;
	}
	std::vector<bool> prime(n);
	eratosthenes_sieve(&prime);
	result.emplace_back(2);
	for (size_t i = 3; i < n; i += 2) {
		if (prime[i]) {
			result.emplace_back(i);
		}
	}
	primes->swap(result);
}

template<typename T>
inline void min_prime_div_vector(const size_t n, std::vector<T>* min_prime_div) {
	std::vector<T> result(n, 0);
	result[0] = result[1] = 1;
	for (size_t i = 2; i < n; i += 2) {
		result[i] = 2;
	}
	size_t index = 3;
	while (index * index < n) {
		if (result[index] == 0) {
			result[index] = index;
			const size_t delta = index << 1;
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
	min_prime_div->swap(result);
}

template<typename T>
inline void min_prime_div_vector(std::vector<T>* min_prime_div) {
	min_prime_div_vector(min_prime_div->size(), min_prime_div);
}
