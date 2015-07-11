#include "StringUtils.h"

void prefixFunction(int p[], const char * s, int n) {
	if (!n) {
		n = strlen(s);
	}
	p[0] = 0;
	for (int i = 1; i < n; ++i) {
		int j = p[i - 1];
		while (j && s[i] != s[j]) {
			j = p[j - 1];
		}
		if (s[i] == s[j]) {
			++j;
		}
		p[i] = j;
	}
}

void prefixFunction(int p[], const string& s) {
	prefixFunction(p, s.c_str(), s.length());
}

void zFunction(int z[], const char * s, int n) {
	if (!n) {
		n = strlen(s);
	}
	z[0] = 0;
	for (int i = 1, l = 0, r = 0; i < n; ++i) {
		z[i] = 0;
		if (i <= r) {
			z[i] = min(r - i + 1, z[i - l]);
		}
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
			++z[i];
		}
		if (i + z[i] - 1 > r) {
			l = i;
			r = i + z[i] - 1;
		}
	}
}

void zFunction(int z[], const string& s) {
	zFunction(z, s.c_str(), s.length());
}
