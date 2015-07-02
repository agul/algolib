#pragma once
#include "Head.h"

inline char upperCase(const char ch);
inline char lowerCase(const char ch);
inline string upperCase(string s);
inline string lowerCase(string s);

inline int dig(const char ch);

inline bool isAlpha(const char ch);
inline bool isDigit(const char ch);
inline bool isLowerCase(const char ch);
inline bool isUpperCase(const char ch);

template<class T> inline string toa(const T x);

void prefixFunction(int p[], const char * s, int n = 0);
void prefixFunction(int p[], const string& s);

void zFunction(int z[], const char * s, int n = 0);
void zFunction(int z[], const string& s);