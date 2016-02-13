#pragma once
#include "Head.h"

inline char upperCase(const char ch) {
	return (ch >= 'a' && ch <= 'z') ? ch ^ 32 : ch;
}

inline char lowerCase(const char ch) {
	return (ch >= 'A' && ch <= 'Z') ? ch ^ 32 : ch;
}

inline std::string upperCase(std::string s) {
	int n = s.length();
	for (int i = 0; i < n; ++i) {
		s[i] = upperCase(s[i]);
	}
	return s;
}

inline std::string lowerCase(std::string s) {
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

inline bool is_digit(const char ch) {
	return (ch >= '0' && ch <= '9');
}

inline bool isLowerCase(const char ch) {
	return (ch >= 'a' && ch <= 'z');
}

inline bool isUpperCase(const char ch) {
	return (ch >= 'A' && ch <= 'Z');
}

template<class T> inline std::string toa(const T x)
// convert string to other types using sstream
{
	std::stringstream ss;
	ss << x;
	std::string ret;
	ss >> ret;
	return ret;
}

void prefixFunction(int p[], const char * s, int n = 0);
void prefixFunction(int p[], const std::string& s);

void zFunction(int z[], const char * s, int n = 0);
void zFunction(int z[], const std::string& s);

void countPalindromes(int d1[], int d2[], const char * s, int n = 0);
void countPalindromes(int d1[], int d2[], const std::string& s);