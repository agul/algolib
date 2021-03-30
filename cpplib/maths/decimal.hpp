#pragma once
#include <functional>
#include <iostream>
#include <string>

#include "safe_double.hpp"

template<typename T = long double, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
class Decimal {
public:
    using value_type = T;

	/// caide keep
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

	Decimal sin() const {
	    return {std::sin(value_)};
	}

	Decimal cos() const {
	    return {std::cos(value_)};
	}

    Decimal tan() const {
        return {std::tan(value_)};
    }

    Decimal atan() const {
        return {std::atan(value_)};
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

    Decimal exp() const {
        return{ std::exp(value_) };
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

	constexpr explicit operator value_type() const {
		return value_;
	}

	constexpr value_type value() const {
		return value_;
	}

	constexpr value_type double_value() const {
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
		return{ static_cast<value_type>(long_value()) };
	}

	constexpr Decimal trunc() const {
		return floor();
	}

	constexpr Decimal round() const {
		return Decimal{value_ + 0.5}.floor();
	}

	std::string str() const {
	    return std::to_string(value_);
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
	value_type value_;
};

template<typename T>
std::string to_string(const Decimal<T> arg) {
    return arg.str();
}

namespace std {

template<typename T>
struct is_floating_point<Decimal<T>> : std::true_type {};

template<typename T>
Decimal<T> abs(const Decimal<T> arg) {
	return arg.abs();
}

template<typename T>
Decimal<T> sqrt(const Decimal<T> arg) {
	return arg.sqrt();
}

template<typename T>
Decimal<T> sin(const Decimal<T> arg) {
    return arg.sin();
}

template<typename T>
Decimal<T> cos(const Decimal<T> arg) {
    return arg.cos();
}

template<typename T>
Decimal<T> tan(const Decimal<T> arg) {
    return arg.tan();
}

template<typename T>
Decimal<T> atan(const Decimal<T> arg) {
    return arg.atan();
}

template<typename T>
Decimal<T> asin(const Decimal<T> arg) {
	return arg.asin();
}

template<typename T>
Decimal<T> acos(const Decimal<T> arg) {
	return arg.acos();
}

template<typename T>
Decimal<T> exp(const Decimal<T> arg) {
    return arg.exp();
}

template<typename T>
Decimal<T> pow(const Decimal<T> lhs, const Decimal<T> rhs) {
	return lhs.pow(rhs);
}

template<typename T>
Decimal<T> log(const Decimal<T> arg) {
	return arg.log();
}

template<typename T>
struct hash<Decimal<T>> {
	size_t operator()(const Decimal<T> arg) const {
		return hash<typename Decimal<T>::value_type>()(arg.value());
	}
};

}  // namespace std