#include "Hasher.h"

void Hasher::hashString(const char * s, int n)
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

void Hasher::hashString(const string& s) {
	hashString(s.c_str(), s.length());
}

ll Hasher::getHash(const int l, const int r) const {
	ll result = (hash[r] - hash[l - 1] * deg[r - l + 1]) % MOD;
	if (result < 0) {
		result += MOD;
	}
	return result;
}

const ll DoubleHasher::BASE[2] = { 277LL, 307LL };
const int DoubleHasher::MOD[2] = { 1000000007, 1000000009 };

void DoubleHasher::hashString(const char * s, int n) {
	if (!n) {
		n = strlen(s);
	}
	for (int i = 0; i < 2; ++i) {
		hash[i][0] = 0;
		for (int j = 0; j < n; ++j) {
			hash[i][j + 1] = (hash[i][j] * BASE[i] + s[j]) % MOD[i];
		}
	}
}

void DoubleHasher::hashString(const string& s) {
	hashString(s.c_str(), s.length());
}

ll DoubleHasher::getHash(const int l, const int r) const {
	ll cur[2];
	for (int i = 0; i < 2; ++i) {
		if ((cur[i] = (hash[i][r] - hash[i][l - 1] * deg[i][r - l + 1]) % MOD[i]) < 0) {
			cur[i] += MOD[i];
		}
	}
	return (cur[0] << 32) ^ cur[1];
}
