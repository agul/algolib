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

void prefixFunction(int p[], const std::string& s) {
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
			z[i] = std::min(r - i + 1, z[i - l]);
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

void zFunction(int z[], const std::string& s) {
	zFunction(z, s.c_str(), s.length());
}

void countPalindromes(int d1[], int d2[], const char * s, int n) {
	if (!n) {
		n = strlen(s);
	}
	std::fill_n(d1, n, 0);
	std::fill_n(d2, n, 0);
	int l = 0, r = -1, k;
	for (int i = 0; i < n; i++){
		if (i > r) k = 1;
		else k = std::min(d1[l + r - i], r - i);

		while (0 <= i - k && i + k < n && s[i - k] == s[i + k]) k++;
		d1[i] = k;
		if (i + k - 1 > r)
			r = i + k - 1, l = i - k + 1;
	}
	l = 0, r = -1;
	for (int i = 0; i < n; i++){
		if (i > r) k = 0;
		else k = std::min(d2[l + r - i + 1], r - i + 1);

		while (i + k < n && i - k - 1 >= 0 && s[i + k] == s[i - k - 1]) k++;
		d2[i] = k;

		if (i + k - 1 > r)
			l = i - k, r = i + k - 1;
	}
}

void countPalindromes(int d1[], int d2[], const std::string& s) {
	countPalindromes(d1, d2, s.c_str(), s.length());
}