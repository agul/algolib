#pragma once
#include <cstdint>
#include <limits>
#include <type_traits>

#ifdef _MSC_VER
#include <intrin.h>
#include <nmmintrin.h>

#define __builtin_popcount __popcnt
#define __builtin_popcountll _mm_popcnt_u64


// bit operations (_builtin_ctz (ctzl, ctzll) and __builtin_clz (clzl, clzll)) for Visual Studio
// https://gist.github.com/pps83/3210a2f980fd02bb2ba2e5a1fc4a2ef0

static inline int __builtin_ctz(unsigned x) {
    unsigned long ret;
    _BitScanForward(&ret, x);
    return (int)ret;
}

static inline int __builtin_ctzll(unsigned long long x) {
    unsigned long ret;
    _BitScanForward64(&ret, x);
    return (int)ret;
}

static inline int __builtin_ctzl(unsigned long x) {
    return sizeof(x) == 8 ? __builtin_ctzll(x) : __builtin_ctz((uint32_t)x);
}

static inline int __builtin_clz(unsigned x) {
    //unsigned long ret;
    //_BitScanReverse(&ret, x);
    //return (int)(31 ^ ret);
    return (int)__lzcnt(x);
}

static inline int __builtin_clzll(unsigned long long x) {
    //unsigned long ret;
    //_BitScanReverse64(&ret, x);
    //return (int)(63 ^ ret);
    return (int)__lzcnt64(x);
}

static inline int __builtin_clzl(unsigned long x) {
    return sizeof(x) == 8 ? __builtin_clzll(x) : __builtin_clz((uint32_t)x);
}

#ifdef __cplusplus
static inline int __builtin_ctzl(unsigned long long x) {
    return __builtin_ctzll(x);
}

static inline int __builtin_clzl(unsigned long long x) {
    return __builtin_clzll(x);
}
#endif
#endif

// ToDo[c++17]: use constexpr if
template<typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
constexpr size_t popcount(const T value) noexcept {
    if (sizeof(T) <= sizeof(uint32_t)) {
        return __builtin_popcount(value);
    } else {
        return __builtin_popcountll(value);
    }
}

template<typename T, typename std::enable_if<!std::is_unsigned<T>::value>::type* = nullptr>
constexpr size_t popcount(const T value) noexcept {
    return popcount(static_cast<typename std::make_unsigned<T>::type>(value));
}

// ToDo[c++17]: use constexpr if
template<typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
constexpr size_t countl_zero(const T value) noexcept {
    constexpr size_t digits = std::numeric_limits<T>::digits;
    if (value == 0) {
        return digits;
    }
    if (sizeof(T) <= sizeof(uint32_t)) {
        return __builtin_clz(value) - (std::numeric_limits<uint32_t>::digits - digits);
    } else {
        return __builtin_clzll(value) - (std::numeric_limits<uint64_t>::digits - digits);
    }
}

template<typename T, typename std::enable_if<!std::is_unsigned<T>::value>::type* = nullptr>
constexpr size_t countl_zero(const T value) noexcept {
    return countl_zero(static_cast<typename std::make_unsigned<T>::type>(value));
}

template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
constexpr size_t countl_one(const T value) noexcept {
    return countl_zero(static_cast<T>(~value));
}

template<typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
constexpr size_t countr_zero(const T value) noexcept {
    if (value == 0) {
        return std::numeric_limits<T>::digits;
    }
    if (sizeof(T) <= sizeof(uint32_t)) {
        return __builtin_ctz(value);
    } else {
        return __builtin_ctzll(value);
    }
}

template<typename T, typename std::enable_if<!std::is_unsigned<T>::value>::type* = nullptr>
constexpr size_t countr_zero(const T value) noexcept {
    return countr_zero(static_cast<typename std::make_unsigned<T>::type>(value));
}

template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
constexpr size_t countr_one(const T value) noexcept {
    return countr_zero(static_cast<T>(~value));
}

template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
constexpr bool has_single_bit(const T value) noexcept {
    return value != 0 && (value & (value - 1)) == 0;
}

template<typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
constexpr T bit_ceil(const T value) noexcept {
    if (value == 0) {
        return 1;
    }
    return static_cast<T>(T{1} << (std::numeric_limits<T>::digits - countl_zero(static_cast<T>(value - 1))));
}

template<typename T, typename std::enable_if<!std::is_unsigned<T>::value>::type* = nullptr>
constexpr T bit_ceil(const T value) noexcept {
    return static_cast<T>(bit_ceil(static_cast<typename std::make_unsigned<T>::type>(value)));
}

template<typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
constexpr T bit_floor(const T value) noexcept {
    if (value == 0) {
        return 0;
    }
    return static_cast<T>(T{1} << (std::numeric_limits<T>::digits - 1 - countl_zero(value)));
}

template<typename T, typename std::enable_if<!std::is_unsigned<T>::value>::type* = nullptr>
constexpr T bit_floor(const T value) noexcept {
    return static_cast<T>(bit_floor(static_cast<typename std::make_unsigned<T>::type>(value)));
}

template<typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
constexpr size_t bit_width(const T value) noexcept {
    return static_cast<T>(std::numeric_limits<T>::digits - countl_zero(value));
}

template<typename T, typename std::enable_if<!std::is_unsigned<T>::value>::type* = nullptr>
constexpr size_t bit_width(const T value) noexcept {
    return bit_width(static_cast<typename std::make_unsigned<T>::type>(value));
}

template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
constexpr bool test_bit(const T mask, const std::size_t bit_id) noexcept {
    return mask & (T{1} << bit_id);
}

template<typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
constexpr size_t binary_power(const T value) {
    if (value == 0) {
        return 0;
    }
    return bit_width(value) - 1;
}
