#pragma once
#include <algorithm>
#include <chrono>
#include <numeric>
#include <random>
#include <string>
#include <type_traits>

template<typename T>
inline T gcd(T a, T b) {
	while (b != 0) {
		a %= b;
		std::swap(a, b);
	}
	return a;
}

template<typename T>
inline T lcm(const T& a, const T& b) {
	return a / gcd(a, b) * b;
}

template<typename T>
constexpr inline T sqr(const T& x) {
	return x * x;
}

template<class T> inline bool isPrime(const T& n)
// Straightforward checking in O(sqrt(N))
{
	if (n < 2) {
		return false;
	}
	T kk = (T)sqrt(n + 0.);
	for (T i = 2; i <= kk; ++i) {
		if (!(n % i)) {
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

template<typename T>
T factorial(T n) {
	T ret = 1;
	for (int i = 2; i <= n; ++i) {
		ret *= i;
	}
	return ret;
}

template<typename T, typename U>
inline T binpow(T a, U b) {
	static_assert(std::is_integral<U>::value, "Degree must be integral. For real degree use pow.");
	T ret = 1;
	while (b != 0) {
		if ((b & 1) != 0) {
			ret *= a;
		}
		a *= a;
		b >>= 1;
	}
	return ret;
}

template<typename T, typename U, typename Q>
inline T binpow(T a, U b, Q mod) {
	static_assert(std::is_integral<U>::value, "Degree must be integral. For real degree use pow.");
	long long ret = 1;
	a %= mod;
	while (b != 0) {
		if ((b & 1) != 0) {
			ret = ret * a % mod;
		}
		a = a * a % mod;
		b >>= 1;
	}
	return ret % mod;
}

template<typename T>
T digit_sum(const T n) {
	T res = 0;
	T tmp = abs(n);
	while (tmp > 0) {
		res += tmp % 10;
		tmp /= 10;
	}
	return res;
}

template<typename T>
std::vector<size_t> digits(const T n) {
	if (n == 0) {
		return{ 0 };
	}
	std::vector<size_t> res;
	T tmp = abs(n);
	while (tmp > 0) {
		res.emplace_back(tmp % 10);
		tmp /= 10;
	}
	return res;
}

template<typename T>
size_t digit_count(const T n) {
	return digits(n).size();
}

template<typename T>
T euler_function(T n) {
	T res = n;
	for (T i = 2; i * i <= n; ++i) {
		if (n % i == 0) {
			while (n % i == 0) {
				n /= i;
			}
			res -= res / i;
		}
	}
	if (n > 1) {
		res -= res / n;
	}
	return res;
}

template<typename T, typename U>
T inverse_element(const T n, const U mod)
// inverse element for prime mod
{
	return binpow(static_cast<long long>(n), mod - 2, mod);
}

inline void inverseElementForSegment(int r[], const int n, const int mod)
// inverse element for prime mod for numbers [1; mod - 1]
{
	r[1] = 1;
	for (int i = 2; i < n; ++i) {
		r[i] = (mod - static_cast<long long>(mod) / i * r[mod % i] % mod) % mod;
	}
}

template<typename T>
T inverse_element_comp_mod(const T n, const T mod)
// inverse element for composite mod using formula inv(n) = n^(phi(mod) - 1)
{
	return binpow(n, euler_function(mod) - 1, mod);
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

template<class T> std::string toRoman(T n) {
	const int ValsCount = 13;
	const int Vals[] = { 1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000 };
	const char * Digits[] = { "I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M" };
	std::string res = "";
	int x = ValsCount - 1;
	while (n) {
		while (Vals[x] > n) {
			--x;
		}
		n -= Vals[x];
		res += Digits[x];
	}
	return res;
}

template<typename T>
void calc_powers(std::vector<T>& deg, const T base) {
	deg[0] = 1;
	for (size_t i = 1; i < deg.size(); ++i) {
		deg[i] = deg[i - 1] * base;
	}
}

inline uint32_t abs(const uint32_t x) {
	return x;
}

inline uint64_t abs(const uint64_t x) {
	return x;
}

namespace Random {

	static std::chrono::system_clock::rep GetRandSeed() {
		return std::chrono::system_clock::now().time_since_epoch().count();
	}

	static std::mt19937_64 gen(GetRandSeed());
	static std::uniform_int_distribution<long long> distrib(0, std::numeric_limits<long long>::max());

	template<typename T>
	static T get(T r) {
		return distrib(gen) % r;
	}

	template<typename T>
	static T get(T l, T r) {
		return get(r - l + 1) + l;
	}

};
