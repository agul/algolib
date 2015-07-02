#pragma once
#include "Head.h"

template<class T> T gcd(T a, T b);
template<class T> inline T lcm(const T a, const T b);

template<class T> inline T sqr(const T x);

template<class T> inline bool isPrime(const T n);

template<class T> inline T ppow(T a, ll b);
template<class T> inline T ppow(T a, ll b, ll md);

template<class T> int digitSum(T n);
template<class T> int digitCount(T n);

template<class T> T eulerFunction(T n);
template<class T> T inverseElement(const T n, const T mod);
void inverseElementForSegment(int r[], const int mod);
template<class T> T inverseElementCompMod(const T n, const T mod);

template<class T, size_t N> void binomialCoefficients(T c[N][N]);
template<class T, size_t N> void binomialCoefficients(T c[N][N], const T mod);

template<class T> string toRoman(T n);