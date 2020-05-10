#pragma once
#include <iostream>
#include <string>
#include <type_traits>

#include "maths.hpp"

constexpr const int BASE_MOD = 1000000007;  /// caide keep

template<typename T>
inline T& add_mod(T& a, const T b, const T mod = BASE_MOD) {
	if ((a += b) >= mod) {
		a -= mod;
	}
	return a;
}

template<typename T>
inline T& sub_mod(T& a, const T b, const T mod = BASE_MOD) {
	if ((a -= b) < 0) {
		a += mod;
	}
	return a;
}

template<typename T>
inline T& mul_mod(T& a, const T b, const T mod = BASE_MOD) {
	a = static_cast<long long>(a) * b % mod;
	return a;
}

template<typename T, T MOD = BASE_MOD>
class ModInt {
public:

	/// caide keep
	constexpr ModInt() : ModInt(0) {}

	constexpr ModInt(const T value) : value_(value) {
		static_assert(MOD > 0, "Modulo must be strictly positive.");
		// static_assert((std::equal<T, int32_t> && mod <= 0x3f3f3f3f) || (std::equal<T, int64_t> && mod <= 0x3f3f3f3f3f3f3f3fLL), "Modulo must be less than half of the max value for typename.");
	}

	template<typename U>
	constexpr static ModInt from_integer(const U value) {
		return{ ModInt::normalize(value) };
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
		return value_ > rhs.value_;
	}

	ModInt operator +(const ModInt rhs) const {
		T x = value_;
		return{ add_mod(x, rhs.value_, MOD) };
	}

	ModInt operator -(const ModInt rhs) const {
		T x = value_;
		return{ sub_mod(x, rhs.value_, MOD) };
	}

	ModInt operator *(const ModInt rhs) const {
		T x = value_;
		return{ mul_mod(x, rhs.value_, MOD) };
	}

	ModInt& operator +=(const ModInt rhs) {
		add_mod(value_, rhs.value_, MOD);
		return *this;
	}

	ModInt& operator -=(const ModInt rhs) {
		sub_mod(value_, rhs.value_, MOD);
		return *this;
	}

	ModInt& operator *=(const ModInt rhs) {
		mul_mod(value_, rhs.value_, MOD);
		return *this;
	}

	ModInt operator ++(int) {
		const ModInt ret(value_);
		add_mod(value_, static_cast<T>(1), MOD);
		return ret;
	}

	ModInt operator --(int) {
		const ModInt ret(value_);
		sub_mod(value_, static_cast<T>(1), MOD);
		return ret;
	}

	ModInt& operator ++() {
		add_mod(value_, static_cast<T>(1), MOD);
		return *this;
	}

	ModInt& operator --() {
		sub_mod(value_, static_cast<T>(1), MOD);
		return *this;
	}

	constexpr ModInt operator +() const {
		return{ value_ };
	}

	constexpr ModInt operator -() const {
		return negate();
	}

	constexpr ModInt negate() const {
		return{ normalize(-value_) };
	}

	constexpr ModInt inverse() const {
		return{ inverse_element(value_, MOD) };
	}

	std::string str() const {
		return std::to_string(value_);
	}

	friend std::istream& operator >>(std::istream& in, ModInt& rhs) {
		T x;
		in >> x;
		rhs.value_ = rhs.normalize(x);
		return in;
	}

	friend std::ostream& operator <<(std::ostream& out, const ModInt& rhs) {
		out << rhs.value_;
		return out;
	}

private:
	T value_;

	template<typename U>
	static T normalize(const U value) {
		if (value >= 0 && value < MOD) {
			return value;
		}
		const T ret = value % MOD;
		if (ret < 0) {
			return ret + MOD;
		}
		return ret;
	}

};

namespace std {

template<typename T, T MOD>
struct is_integral<ModInt<T, MOD>> : std::true_type {};

template<typename T, T MOD>
struct is_arithmetic<ModInt<T, MOD>> : std::true_type {};

}  // namespace std

template<typename T, T MOD>
std::string to_string(const ModInt<T, MOD>& val) {
	return val.str();
}
