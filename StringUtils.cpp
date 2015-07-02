#include "StringUtils.h"

inline char upperCase(const char ch) {
	return (ch >= 'a' && ch <= 'z') ? ch ^ 32 : ch;
}

inline char lowerCase(const char ch) {
	return (ch >= 'A' && ch <= 'Z') ? ch ^ 32 : ch;
}

inline string upperCase(string s) {
	int n = s.length();
	for (int i = 0; i < n; ++i) {
		s[i] = upperCase(s[i]);
	}
	return s;
}

inline string lowerCase(string s) {
	int n = s.length();
	for (int i = 0; i < n; ++i) {
		s[i] = lowerCase(s[i]);
	}
	return s;
}

inline int dig(const char ch) 
// get exact numeric value of the corresponding char
// REQUIRE: char from '0' to '9'
{
	return ch - '0';
}

inline bool isAlpha(const char ch) {
	return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}

inline bool isDigit(const char ch) {
	return (ch >= '0' && ch <= '9');
}

inline bool isLowerCase(const char ch) {
	return (ch >= 'a' && ch <= 'z');
}

inline bool isUpperCase(const char ch) {
	return (ch >= 'A' && ch <= 'Z');
}

template<class T> inline string toa(const T x)
// convert string to other types using sstream
{
	stringstream ss;
	ss << x;
	string ret;
	ss >> ret;
	return ret;
}

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
