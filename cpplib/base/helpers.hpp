#pragma once
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <utility>

#include "base/constants.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#define popcount(a) __popcnt(a)
#define clz(a) __clz(a)
#define ctz(a) __ctz(a)
#else
#define popcount(a) __builtin_popcount(a)
#define clz(a) __builtin_clz(a)
#define ctz(a) __builtin_ctz(a)
#endif

template<typename T>
inline bool is_equal_to_zero(const T& a) {
	if (std::is_floating_point<T>()) {
		return abs(a) < EPS;
	}
	return a == 0;
}

template<typename T>
inline bool umin(T& a, const T& b) {
	return (b < a ? a = b, true : false);
}

template<typename T>
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

#define fill(a, x) memset(a, x, sizeof(a))
#define sz(a) ((int)a.size())
#define X first
#define Y second
#define mp std::make_pair
#define all(a) a.begin(), a.end()
#define pb push_back
