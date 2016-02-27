#pragma once
#pragma comment(linker, "/STACK:66777216")
#define _USE_MATH_DEFINES
#include <cstdio>
#pragma warning(disable : 4996)
#include <algorithm>
#include <array>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <string>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <utility>
#include <functional>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cassert>
#include <queue>
#include <cmath>
#include <random>
#include <sstream>
#include <numeric>
#include <limits>
#include <chrono>
#include <type_traits>
#pragma hdrstop

#ifdef _MSC_VER
#include <intrin.h>
#define LLD "%I64d"
#define LLU "%I64u"
#define popcount(a) __popcnt(a)
#define clz(a) __clz(a)
#define ctz(a) __ctz(a)
#else
#define LLD "%lld"
#define LLU "%llu"
#define popcount(a) __builtin_popcount(a)
#define clz(a) __builtin_clz(a)
#define ctz(a) __builtin_ctz(a)
#endif

#define fill(a, x) memset(a, x, sizeof(a))
#define sz(a) ((int)a.size())
#define X first
#define Y second
#define mp std::make_pair
#define all(a) a.begin(), a.end()
#define pb push_back

#define ll int64_t
#define ull uint64_t
#define uint unsigned int
#define pii std::pair<int, int>
#define pll std::pair<ll, ll>
#define pli std::pair<ll, int>
#define vi std::vector<int>
#define vvi std::vector<vi>
#define vp std::vector<pii>
#define ld long double

const long double PI = 3.14159265358979323846;
const int INF = 0x3f3f3f3f;
const double EPS = 1e-9;
const ll LINF = 0x3f3f3f3f3f3f3f3fLL;

const int DX[] = { -1, 0, 1, 0, -1, -1, 1, 1};
const int DY[] = { 0, -1, 0, 1, -1, 1, 1, -1};

template<class T>
inline bool is_equal_to_zero(const T& a) {
	if (std::is_floating_point<T>()) {
		return abs(a) < EPS;
	}
	return a == 0;
}


template<class T> 
inline T& add_mod(T& a, const T& b, const T& mod = 1000000007) {
	if ((a += b) >= mod) {
		a -= mod;
	}
	return a;
}

template<class T> 
inline T& sub_mod(T& a, const T& b, const T& mod = 1000000007) {
	if ((a -= b) < 0) {
		a += mod;
	}
	return a;
}

template<class T>
inline T& mul_mod(T& a, const T& b, const T& mod = 1000000007) {
	a = a * b % mod;
	return a;
}

template<class T> 
inline bool umin(T& a, const T& b) {
	return (b < a ? a = b, true : false);
}

template<class T> 
inline bool umax(T& a, const T& b) {
	return (a < b ? a = b, true : false);
}

namespace std {

	template<typename T, typename U>
	struct hash<std::pair<T, U>> {
		std::size_t operator()(const std::pair<T, U>& lhs) const {
			return lhs.first * 877117 + lhs.second;
		}
	};

}

#ifdef _MSC_VER
static uint32_t __inline __clz(uint32_t x) {
	unsigned long r = 0;
	_BitScanReverse(&r, x);
	return (31 - r);
}

static uint32_t __inline __ctz(uint32_t x) {
	unsigned long r = 0;
	_BitScanForward(&r, x);
	return r;
}
#endif
