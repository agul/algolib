#pragma once
#include <algorithm>
#include <numeric>
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

template<typename T>
T extended_gcd(const T a, const T b, T& x, T& y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    const T p = b / a;
    const T g = extended_gcd(b - p * a, a, y, x);
    x -= p * y;
    return g;
}

// |x|, |y| <= max(|a|, |b|, |c|)
template<typename T>
bool solve_diophantine(const T a, const T b, const T c, T& x, T& y, T& g) {
    if (a == 0 && b == 0) {
        if (c == 0) {
            x = 0;
            y = 0;
            g = 0;
            return true;
        }
        return false;
    }
    if (a == 0) {
        if (c % b == 0) {
            x = 0;
            y = c / b;
            g = std::abs(b);
            return true;
        }
        return false;
    }
    if (b == 0) {
        if (c % a == 0) {
            x = c / a;
            y = 0;
            g = std::abs(a);
            return true;
        }
        return false;
    }
    g = extended_gcd(a, b, x, y);
    if (c % g != 0) {
        return false;
    }
    T adjusted_c = c;
    const T dx = adjusted_c / a;
    adjusted_c -= dx * a;
    const T dy = adjusted_c / b;
    adjusted_c -= dy * b;
    x = dx + static_cast<T>(static_cast<__int128>(x) * (adjusted_c / g) % b);
    y = dy + static_cast<T>(static_cast<__int128>(y) * (adjusted_c / g) % a);
    g = std::abs(g);
    return true;
}

template<typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
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

template<typename T>
T factorial(const size_t n) {
	T ret = 1;
	for (size_t i = 2; i <= n; ++i) {
		ret *= i;
	}
	return ret;
}

template<typename T, typename U>
inline T binpow(T a, U b) {
	static_assert(std::is_integral<U>::value, "Degree must be integral. For real degree use pow().");
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
	static_assert(std::is_integral<U>::value, "Degree must be integral. For real degree use pow().");
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
std::vector<T> divisors_vector(const T n) {
	const auto approximate_divisors_count = static_cast<size_t>(std::cbrt(n));
	std::vector<T> ans;
	ans.reserve(approximate_divisors_count);
	const auto floored_square_root = static_cast<T>(sqrt(n));
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

inline uint32_t abs(const uint32_t x) {
	return x;
}

inline uint64_t abs(const uint64_t x) {
	return x;
}
