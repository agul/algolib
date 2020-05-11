#pragma once
#include <cstdlib>

#include "maths/maths.hpp"
#include "string/string_view.hpp"

#include "double_hasher.hpp"

template<class T>
class Hasher {
public:
	T * deg, * hash;
	int P;
	StringView str;
	int n;

	explicit Hasher(const int n, const int P = std::max(rand(), 307)) : n(n), P(P) {
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
		int L = 0, R = n - std::max(i, j) + 1;
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
		int LEN = n - std::max(a, b), L = lcp(a, b);
		return L < LEN ? (int)str[a + L] - str[b + L] : b - a;
	}

};
