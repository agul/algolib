#pragma once
#include "Head.h"

template<class T> T gcd(T a, T b) {
	while (b) {
		a %= b;
		swap(a, b);
	}
	return a;
}

template<class T> inline T lcm(const T& a, const T& b) {
	return a / gcd(a, b) * b;
}

template<class T> inline T sqr(const T& x) {
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

inline void eratosthenesSieve(bool prime[], const int n) {
	if (n < 2) {
		std::fill_n(prime, n, false);
		return;
	}
	std::fill_n(prime, n, true);
	prime[0] = prime[1] = false;
	for (int i = 4; i < n; i += 2) {
		prime[i] = false;
	}
	for (ll i = 3; i * i < n; i += 2) {
		if (prime[i]) {
			ll ii = i << 1;
			for (ll j = i * i; j < n; j += ii) {
				prime[j] = false;
			}
		}
	}
}

inline void primesVector(bool prime[], const int n, int primes[], int& primesCnt) {
	if (n < 2) {
		primesCnt = 0;
		return;
	}
	primesCnt = 1;
	primes[0] = 2;
	for (int i = 3; i < n; i += 2) {
		if (prime[i]) {
			primes[primesCnt++] = i;
		}
	}
}

template<class T>
T factorial(T n, const T& mod = 1000000007) {
	ll ret = 1;
	for (int i = 2; i <= n; ++i) {
		ret = (ret * i) % mod;
	}
	return ret % mod;
}

template<class T>
inline T ppow(T a, T b) {
	T ret = 1;
	while (b) {
		if (b & 1) {
			ret *= a;
		}
		a *= a;
		b >>= 1;
	}
	return ret;
}

template<class T>
inline T ppow(T a, T b, T md) {
	ll ret = 1;
	a %= md;
	while (b) {
		if (b & 1) {
			ret = ret * a % md;
		}
		a = a * a % md;
		b >>= 1;
	}
	return ret % md;
}

template<class T>
int digitSum(T n) {
	int res = 0;
	n = abs(n);
	while (n) {
		res += n % 10;
		n /= 10;
	}
	return res;
}

template<class T>
int digitCount(T n) {
	int res = 0;
	n = abs(n);
	while (n) {
		++res;
		n /= 10;
	}
	return res;
}

template<class T> T eulerFunction(T n) {
	T res = n;
	for (T i = 2; i * i <= n; ++i) {
		if (!(n % i)) {
			while (!(n % i)) {
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

template<class T> T inverseElement(const T n, const T mod)
// inverse element for prime mod
{
	return ppow(n, mod - 2, mod);
}

inline void inverseElementForSegment(int r[], const int mod)
// inverse element for prime mod for numbers [1; mod - 1]
{
	r[1] = 1;
	for (int i = 2; i < mod; ++i) {
		r[i] = (mod - mod / i * r[mod % i] % mod) % mod;
	}
}

template<class T> T inverseElementCompMod(const T n, const T mod)
// inverse element for composite mod using formula inv(n) = n^(phi(mod) - 1)
{
	return ppow(n, eulerFunction(mod) - 1, mod);
}

template<class T, size_t N> void binomialCoefficients(T (&c)[N][N]) {
	for (int i = 0; i < N; ++i) {
		c[i][0] = c[i][i] = 1;
		for (int j = 1; j < i; ++j) {
			c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
		}
	}
}

template<class T, size_t N> void binomialCoefficients(T (&c)[N][N], const T mod) {
	for (int i = 0; i < N; ++i) {
		c[i][0] = c[i][i] = 1;
		for (int j = 1; j < i; ++j) {
			if ((c[i][j] = c[i - 1][j - 1] + c[i - 1][j]) >= mod) {
				c[i][j] -= mod;
			}
		}
		for (int j = i + 1; j < N; ++j) {
			c[i][j] = 0;
		}
	}
}

template<class T> std::string toRoman(T n) {
	const int ValsCount = 13;
	const int Vals[] = { 1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000 };
	const char * Digits[] = { "I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M" };
	string res = "";
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

void calcPowers(ll deg[], ll base, int n);

template<class T> void calcPowers(T deg[], T base, int n, T MOD) {
	deg[0] = 1 % MOD;
	for (int i = 1; i <= n; ++i) {
		deg[i] = (ll)deg[i - 1] * base % MOD;
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
	static std::uniform_int_distribution<int64_t> distrib(0, std::numeric_limits<int64_t>::max());

	template<typename T>
	static T get(T r) {
		return distrib(gen) % r;
	}

	template<typename T>
	static T get(T l, T r) {
		return get(r - l + 1) + l;
	}

};
