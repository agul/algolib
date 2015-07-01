#pragma once
#include "Head.h"

template<class T> T gcd(T a, T b) {
	while (b) {
		a %= b;
		swap(a, b);
	}
	return a;
}

template<class T> inline T lcm(const T a, const T b) {
	return a / gcd(a, b) * b;
}

template<class T> inline T sqr(const T x) {
	return x * x;
}

template<class T> inline bool isPrime(const T n) {
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

template<class T> inline T ppow(T a, ll b) {
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

template<class T> inline T ppow(T a, ll b, ll md) {
	T ret = 1;
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

template<class T> int digitSum(T n) {
	int res = 0;
	n = abs(n);
	while (n) {
		res += n % 10;
		n /= 10;
	}
	return res;
}

template<class T> int digitCount(T n) {
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


void inverseElementForSegment(int r[], const int mod)
// inverse element for prime mod for numbers [1; mod - 1]
{
	r[1] = 1;
	for (int i = 2; i < mod; ++i) {
		r[i] = (mod - mod / i * r[mod % i] % mod) % mod;
	}
}

template<class T> T inverseElementCompMod(const T n, const T mod) {
	return ppow(n, eulerFunction(mod) - 1, mod);
}

template<class T, size_t N> void binomialCoefficients(T c[N][N]) {
	for (int i = 0; i < N; ++i) {
		c[i][0] = c[i][i] = 1;
		for (int j = 1; j < i; ++j) {
			c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
		}
	}
}

template<class T, size_t N> void binomialCoefficients(T c[N][N], const T mod) {
	for (int i = 0; i < N; ++i) {
		c[i][0] = c[i][i] = 1;
		for (int j = 1; j < i; ++j) {
			if ((c[i][j] = c[i - 1][j - 1] + c[i - 1][j]) >= mod) {
				c[i][j] -= mod;
			}
		}
		for (int j = i + 1; j < n; ++j) {
			c[i][j] = 0;
		}
	}
}

template<class T> string toRoman(T n) {
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