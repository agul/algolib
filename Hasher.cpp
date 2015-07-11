#include "Hasher.h"

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

void SingleHasher::hashString(const string& s) {
	hashString(s.c_str(), s.length());
}

ll SingleHasher::getHash(const int l, const int r) const {
	ll result = (hash[r] - hash[l - 1] * deg[r - l + 1]) % MOD;
	if (result < 0) {
		result += MOD;
	}
	return result;
}
