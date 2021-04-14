#pragma once
#include <iostream>
#include <string>
#include <type_traits>

#include "maths.hpp"
#include "euler_function.hpp"

constexpr const int kBaseModulo = 1000000007;  /// caide keep

template<typename T>
inline T& add_mod(T& a, const T b, const T mod = kBaseModulo) {
	if ((a += b) >= mod) {
		a -= mod;
	}
	return a;
}

template<typename T>
inline T& sub_mod(T& a, const T b, const T mod = kBaseModulo) {
	if ((a -= b) < 0) {
		a += mod;
	}
	return a;
}

template<typename T>
inline T& mul_mod(T& a, const T b, const T mod = kBaseModulo) {
	a = static_cast<int64_t>(a) * b % mod;
	return a;
}

template<typename T, typename U, typename Q>
inline T binpow(T a, U b, Q mod) {
    static_assert(std::is_integral<U>::value, "Degree must be integral. For real degree use pow().");
    int64_t ret = 1;
    a %= mod;
    while (b != 0) {
        if ((b & 1) != 0) {
            ret = ret * a % mod;
        }
        a = a * a % mod;
        b >>= 1;
    }
    return ret % mod;
}

template<typename T, typename U>
T inverse_element(const T n, const U mod)
// inverse element for prime mod
{
    return binpow(static_cast<int64_t>(n), mod - 2, mod);
}

template<typename T, typename U>
std::vector<T> inverse_element_for_segment(const size_t n, const U mod)
// inverse element for prime mod for numbers [1; n - 1]
{
    std::vector<T> res(n + 1);
    res[1] = 1;
    for (size_t i = 2; i < res.size(); ++i) {
        res[i] = (mod -  static_cast<int64_t>(mod) / i * res[mod % i] % mod) % mod;
    }
    return res;
}

template<typename T>
T inverse_element_comp_mod(const T n, const T mod)
// inverse element for composite mod using formula inv(n) = n^(phi(mod) - 1)
{
    return binpow(n, euler_function(mod) - 1, mod);
}

template<typename T, typename U>
std::vector<T> calc_inverse_factorial(const size_t n, const U mod) {
    const std::vector<T> inv = inverse_element_for_segment<T>(n, mod);
    std::vector<T> inversed_factorial(n + 1);
    inversed_factorial[0] = 1;
    inversed_factorial[1] = 1;
    for (size_t i = 2; i < inversed_factorial.size(); ++i) {
        inversed_factorial[i] = inversed_factorial[i - 1];
        mul_mod(inversed_factorial[i], inv[i], mod);
    }
    return inversed_factorial;
}

template<typename T, T MOD = kBaseModulo>
class ModInt {
public:
	using value_type = T;

	static constexpr value_type Modulo = MOD;

	/// caide keep
	constexpr ModInt() : ModInt(0) {}

	constexpr ModInt(const value_type value) : value_(value) {
		static_assert(MOD > 0, "Modulo must be strictly positive.");
		// static_assert((std::equal<value_type, int32_t> && MOD <= 0x3f3f3f3f) || (std::equal<value_type, int64_t> && MOD <= 0x3f3f3f3f3f3f3f3fLL), "Modulo must be less than half of the max value for typename.");
	}

	template<typename U>
	constexpr static ModInt from_integer(const U value) {
		return {ModInt::normalize(value)};
	}

	constexpr value_type value() const {
		return value_;
	}

	constexpr value_type mod() const {
		return MOD;
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
		value_type x = value_;
		return {add_mod(x, rhs.value_, MOD)};
	}

	ModInt operator -(const ModInt rhs) const {
		value_type x = value_;
		return {sub_mod(x, rhs.value_, MOD)};
	}

	ModInt operator *(const ModInt rhs) const {
		value_type x = value_;
		return {mul_mod(x, rhs.value_, MOD)};
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
		add_mod(value_, static_cast<value_type>(1), MOD);
		return ret;
	}

	ModInt operator --(int) {
		const ModInt ret(value_);
		sub_mod(value_, static_cast<value_type>(1), MOD);
		return ret;
	}

	ModInt& operator ++() {
		add_mod(value_, static_cast<value_type>(1), MOD);
		return *this;
	}

	ModInt& operator --() {
		sub_mod(value_, static_cast<value_type>(1), MOD);
		return *this;
	}

	constexpr ModInt operator +() const {
		return{ value_ };
	}

	constexpr ModInt operator -() const {
		return negate();
	}

	constexpr ModInt negate() const {
		return {ModInt::normalize(-value_)};
	}

	constexpr ModInt inverse() const {
		return {inverse_element(value_, MOD)};
	}

	std::string str() const {
		return std::to_string(value_);
	}

	template<typename C>
	C as() const {
		return static_cast<C>(value_);
	}

	explicit operator int() const {
	    return as<int32_t>();
	}

	explicit operator long long() const {
	    return as<int64_t>();
	}

	friend std::istream& operator >>(std::istream& in, ModInt& rhs) {
		value_type x;
		in >> x;
		rhs.value_ = ModInt::normalize(x);
		return in;
	}

	friend std::ostream& operator <<(std::ostream& out, const ModInt& rhs) {
		out << rhs.value_;
		return out;
	}

private:
	value_type value_;

	template<typename U>
	static value_type normalize(const U value) {
		if (value >= 0 && value < MOD) {
			return value;
		}
		const value_type ret = value % MOD;
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
