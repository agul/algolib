#pragma once
#include <functional>
#include <iostream>

#include "safe_double.hpp"

class Decimal {
public:
	using T = long double;

	constexpr Decimal() : Decimal(0) {}
	constexpr Decimal(const T value) : value_(value) {}

	Decimal& operator +=(const Decimal& rhs) {
		value_ += rhs.value_;
		return *this;
	}

	Decimal& operator -=(const Decimal& rhs) {
		value_ -= rhs.value_;
		return *this;
	}

	Decimal& operator *=(const Decimal& rhs) {
		value_ *= rhs.value_;
		return *this;
	}

	Decimal& operator /=(const Decimal& rhs) {
		value_ /= rhs.value_;
		return *this;
	}

	constexpr Decimal operator +(const Decimal& rhs) const {
		return{ value_ + rhs.value_ };
	}

	constexpr Decimal operator -(const Decimal& rhs) const {
		return{ value_ - rhs.value_ };
	}

	constexpr Decimal operator *(const Decimal& rhs) const {
		return{ value_ * rhs.value_ };
	}

	constexpr Decimal operator /(const Decimal& rhs) const {
		return{ value_ / rhs.value_ };
	}

	constexpr Decimal operator +() const {
		return{ value_ };
	}

	constexpr Decimal operator -() const {
		return negate();
	}

	constexpr Decimal negate() const {
		return{ -value_ };
	}

	constexpr Decimal abs() const {
		return{ safe::abs(value_) };
	}

	Decimal sqrt() const {
		return{ safe::sqrt(value_) };
	}

	Decimal asin() const {
		return{ safe::asin(value_) };
	}

	Decimal acos() const {
		return{ safe::acos(value_) };
	}

	Decimal pow(const Decimal arg) const {
		return{ std::pow(value_, arg.value()) };
	}

	Decimal log() const {
		return{ std::log(value_) };
	}

	constexpr bool operator ==(const Decimal& rhs) const {
		return safe::equal(value_, rhs.value_);
	}

	constexpr bool operator !=(const Decimal& rhs) const {
		return safe::not_equal(value_, rhs.value_);
	}

	constexpr bool operator <(const Decimal& rhs) const {
		return safe::less(value_, rhs.value_);
	}

	constexpr bool operator <=(const Decimal& rhs) const {
		return safe::less_or_equal(value_, rhs.value_);
	}

	constexpr bool operator >(const Decimal& rhs) const {
		return safe::greater(value_, rhs.value_);
	}

	constexpr bool operator >=(const Decimal& rhs) const {
		return safe::greater_or_equal(value_, rhs.value_);
	}

	constexpr explicit operator T() const {
		return value_;
	}

	constexpr T value() const {
		return value_;
	}

	constexpr T double_value() const {
		return value_;
	}

	constexpr int int_value() const {
		return static_cast<int>(value_);
	}

	constexpr long long long_value() const {
		return static_cast<long long>(value_);
	}

	constexpr Decimal frac() const {
		return{ value_ - long_value() };
	}

	Decimal ceil() const {
		if (safe::is_zero(frac())) {
			return{ value_ };
		}
		return{ long_value() + 1.0 };
	}

	constexpr Decimal floor() const {
		return{ static_cast<T>(long_value()) };
	}

	constexpr Decimal trunc() const {
		return floor();
	}

	constexpr Decimal round() const {
		return Decimal(value_ + 0.5).floor();
	}

	static Decimal pi() {
		return{ PI };
	}

	static Decimal zero() {
		return{ 0 };
	}

	friend std::istream& operator >>(std::istream& in, Decimal& x) {
		in >> x.value_;
		return in;
	}

	friend std::ostream& operator <<(std::ostream& out, const Decimal& x) {
		out << x.value_;
		return out;
	}

private:
	T value_;

};

namespace std {

template<>
struct is_floating_point<Decimal> : std::true_type {};

Decimal abs(const Decimal arg) {
	return arg.abs();
}

Decimal sqrt(const Decimal arg) {
	return arg.sqrt();
}

Decimal asin(const Decimal arg) {
	return arg.asin();
}

Decimal acos(const Decimal arg) {
	return arg.acos();
}

Decimal pow(const Decimal lhs, const Decimal rhs) {
	return lhs.pow(rhs);
}

Decimal log(const Decimal arg) {
	return arg.log();
}

template<>
struct hash<Decimal> {
	size_t operator()(const Decimal arg) const {
		return hash<Decimal::T>()(arg.value());
	}
};

}