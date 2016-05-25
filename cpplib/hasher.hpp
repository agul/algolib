#pragma once
#include "Head.h"
#include "Maths.h"
#include "StringView.h"

class SingleHasher {
public:

	static const ll BASE = 277;
	static const int MOD = 1000000007;

	ll * hash, *deg;

	SingleHasher(const int n) {
		hash = new ll[n + 1];
		deg = new ll[n + 1];
		calcPowers(deg, BASE, n);
	}

	~SingleHasher() {
		delete[] hash;
		delete[] deg;
	}

	void hashString(const char * s, int n = 0);
	void hashString(const std::string& s);

	ll getHash(const int l, const int r) const;

private:
	SingleHasher();

};

class DoubleHasher_t {
public:
	static const uint32_t MA = static_cast<uint32_t>(1e9) + 7;
	static const uint32_t MB = static_cast<uint32_t>(1e9) + 9;

	int a, b;

	DoubleHasher_t() {}
	DoubleHasher_t(const int x) : a(x), b(x) {}
	DoubleHasher_t(const int a, const int b) : a(a), b(b) {}

	DoubleHasher_t operator + (const DoubleHasher_t &x) const {
		return DoubleHasher_t((a + x.a) % MA, (b + x.b) % MB);
	}

	DoubleHasher_t operator - (const DoubleHasher_t &x) const {
		return DoubleHasher_t((a + MA - x.a) % MA, (b + MB - x.b) % MB);
	}

	DoubleHasher_t operator * (const int x) const {
		return DoubleHasher_t(((ll)a * x) % MA, ((ll)b * x) % MB);
	}

	DoubleHasher_t operator * (const DoubleHasher_t &x) const {
		return DoubleHasher_t(((ll)a * x.a) % MA, ((ll)b * x.b) % MB);
	}

	bool operator == (const DoubleHasher_t &x) const {
		return a == x.a && b == x.b;
	}

	bool operator < (const DoubleHasher_t& x) {
		return a < x.a || (a == x.a && b < x.b);
	}

	explicit operator ll() const {
		return (ll)a * MB + b + 1;
	}

};

template<class T> class Hasher {
public:

	T * deg, * hash;
	int P;
	StringView str;

	Hasher(const int n, const int P = std::max(rand(), 307)) : P(P) {
		hash = new T[n + 1];
		deg = new T[n + 1];
		deg[0] = 1;
		for (int i = 1; i <= n; ++i) {
			deg[i] = deg[i - 1] * P;
		}
	}

	~Hasher() {
		delete[] hash;
		delete[] deg;
	}

	void hashString(const StringView& s) {
		str = s;
		hash[0] = 0;
		for (int i = 0; i < s.length(); ++i) {
			hash[i + 1] = hash[i] * P + s[i];
		}
	}

	template<typename U>
	void hash_vector(const std::vector<U>& vec) {
		hash[0] = 0;
		for (int i = 0; i < vec.size(); ++i) {
			hash[i + 1] = hash[i] * P + vec[i];
		}
	}

	T getHash(const int l, const int r) const {
		return hash[r] - hash[l - 1] * deg[r - l + 1];
	}

	int lcp(const int i, const int j) const {
		int L = 0, R = n - max(i, j);
		while (L < R) {
			int M = (L + R + 1) >> 1;
			if (getHash(i, i + M - 1) == getHash(j, j + M - 1)) {
				L = M;
			} else {
				R = M - 1;
			}
		}
		return L;
	}

	bool less(const int i, const int j) const {
		return cmpSubstrings(i, j) < 0;
	}

private:
	Hasher();

	int cmpSubstrings(int a, int b) const {
		int LEN = n - max(a, b), L = lcp(a, b);
		return L < LEN ? (int)str[a + L] - str[b + L] : b - a;
	}

};

void SingleHasher::hashString(const char * s, int n)
// hash begins at index 1
{
	if (!n) {
		n = strlen(s);
	}
	hash[0] = 0;
	for (int i = 0; i < n; ++i) {
		hash[i + 1] = (hash[i] * BASE + s[i]) % MOD;
	}
}

void SingleHasher::hashString(const std::string& s) {
	hashString(s.c_str(), s.length());
}

ll SingleHasher::getHash(const int l, const int r) const {
	ll result = (hash[r] - hash[l - 1] * deg[r - l + 1]) % MOD;
	if (result < 0) {
		result += MOD;
	}
	return result;
}
