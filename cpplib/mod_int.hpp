#pragma once
#include "Head.h"

template<typename T>
constexpr inline T& add_mod(T& a, const T b, const T mod = 1000000007) {
	if ((a += b) >= mod) {
		a -= mod;
	}
	return a;
}

template<typename T>
constexpr inline T& sub_mod(T& a, const T b, const T mod = 1000000007) {
	if ((a -= b) < 0) {
		a += mod;
	}
	return a;
}

template<typename T>
constexpr inline T& mul_mod(T& a, const T b, const T mod = 1000000007) {
	a = a * b % mod;
	return a;
}

template<typename T, T MOD = 1000000007>
class ModInt {
public:

	constexpr ModInt() : ModInt(0) {}

	template<typename U>
	constexpr explicit ModInt(const U value) : value_(normalize(value)) {
		static_assert(MOD > 0, "Modulo must be strictly positive.");
		// static_assert((std::equal<T, int32_t> && mod <= 0x3f3f3f3f) || (std::equal<T, int64_t> && mod <= 0x3f3f3f3f3f3f3f3fLL), "Modulo must be less than half of the max value for typename.");
	}

	constexpr T value() const {
		return value_;
	}

	constexpr bool operator ==(const ModInt rhs) const {
		return value_ == rhs.value_;
	}

	constexpr bool operator !=(const ModInt rhs) const {
		return !operator==(rhs);
	}

	constexpr bool operator <(const ModInt& rhs) const {
		return value_ < rhs.value_;
	}

	constexpr bool operator >(const ModInt& rhs) const {
		return value > rhs.value_;
	}

	constexpr ModInt operator +(const ModInt rhs) const {
		T x = value_;
		return{ add_mod(x, rhs.value_, MOD) };
	}

	constexpr ModInt operator -(const ModInt rhs) const {
		T x = value_;
		return{ sub_mod(x, rhs.value_, MOD) };
	}

	constexpr ModInt operator *(const ModInt rhs) const {
		T x = value_;
		return{ mul_mod(x, rhs.value_, MOD) };
	}

	constexpr ModInt& operator +=(const ModInt rhs) {
		add_mod(value_, rhs.value_, MOD);
		return *this;
	}

	constexpr ModInt& operator -=(const ModInt rhs) {
		sub_mod(value_, rhs.value_, MOD);
		return *this;
	}

	constexpr ModInt& operator *=(const ModInt rhs) {
		mul_mod(value_, rhs.value_, MOD);
		return *this;
	}

	constexpr ModInt operator ++() {
		const ModInt ret(value_);
		add_mod(value_, 1, MOD);
		return ret;
	}

	constexpr ModInt operator --() {
		const ModInt ret(value_);
		sub_mod(value_, 1, MOD);
		return ret;
	}

	constexpr ModInt operator ++(int) {
		add_mod(value_, 1, MOD);
		return{ value_ };
	}

	constexpr ModInt operator --(int) {
		sub_mod(value_, 1, MOD);
		return{ value_ };
	}

private:
	T value_;

	template<typename U>
	constexpr T normalize(const U value) const {
		T ret = value % MOD;
		if (ret < 0) {
			return ret + MOD;
		}
		return ret;
	}

};