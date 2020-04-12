#pragma once
#include <functional>
#include <string>
#include <type_traits>

#include "../maths/maths.hpp"

template<typename T>
class Rational {
public:
	static_assert(std::is_integral<T>::value, "Base type of Rational must be an integral.");

	constexpr Rational() : Rational(0) {}
	constexpr Rational(const T a) : a_(a), b_(1) {}
	Rational(const T a, const T b) : a_(a), b_(b) {
		normalize();
	}

	void swap(Rational& rhs) {
		std::swap(a_, rhs.a_);
		std::swap(b_, rhs.b_);
	}

	constexpr operator bool() const {
		return a_ != 0;
	}

	constexpr operator T() const {
		return to_int();
	}

	constexpr operator long double() const {
		return to_double();
	}

	std::string to_string() const {
		return std::to_string(a_) + "/" + std::to_string(b_);
	}

	constexpr bool operator !() const {
		return a_ == 0;
	}

	constexpr Rational operator -() const {
		return negate();
	}

	constexpr Rational operator +() const {
		return{ a_, b_ };
	}

	Rational& operator ++() {
		a_ += b_;
		return *this;
	}

	Rational operator ++(int) {
		a_ += b_;
		return {a_ - b_, b_};
	}

	Rational& operator --() {
		a_ -= b_;
		return *this;
	}

	Rational operator --(int) {
		a_ -= b_;
		return{ a_ + b_, b_ };
	}

	constexpr Rational add(const Rational& rhs) const {
		return{ a_ * rhs.b_ + rhs.a_ * b_, b_ * rhs.b_ };
	}

	constexpr Rational subtract(const Rational& rhs) const {
		return add(rhs.negate());
	}

	constexpr Rational multiply(const Rational& rhs) const {
		return{ a_ * rhs.a_, b_ * rhs.b_ };
	}

	constexpr Rational divide(const Rational& rhs) const {
		return{ a_ * rhs.b_, b_ * rhs.a_ };
	}

	constexpr Rational operator +(const Rational& rhs) const {
		return add(rhs);
	}

	constexpr Rational operator -(const Rational& rhs) const {
		return subtract(rhs);
	}

	constexpr Rational operator *(const Rational& rhs) const {
		return multiply(rhs);
	}

	constexpr Rational operator /(const Rational& rhs) const {
		return divide(rhs);
	}

	constexpr T a() const {
		return a_;
	}

	constexpr T b() const {
		return b_;
	}

	Rational& operator +=(const Rational& rhs) {
		Rational res = add(rhs);
		swap(res);
		return *this;
	}

	Rational& operator -=(const Rational& rhs) {
		Rational res = subtract(rhs);
		swap(res);
		return *this;
	}

	Rational& operator *=(const Rational& rhs) {
		Rational res = multiply(rhs);
		swap(res);
		return *this;
	}

	Rational& operator /=(const Rational& x) {
		Rational res = divide(rhs);
		swap(res);
		return *this;
	}

	constexpr friend bool operator ==(const Rational& lhs, const Rational& rhs) {
		return lhs.a_ == rhs.a_ && lhs.b_ == rhs.b_;
	}

	constexpr friend bool operator !=(const Rational& lhs, const Rational& rhs) {
		return !(lhs == rhs);
	}

	constexpr friend bool operator <(const Rational& lhs, const Rational& rhs) {
		return lhs.a_ * rhs.b_ < rhs.a_ * lhs.b_;
	}

	constexpr friend bool operator >(const Rational& lhs, const Rational& rhs) {
		return rhs < lhs;
	}

	constexpr friend bool operator <=(const Rational& lhs, const Rational& rhs) {
		return lhs.a_ * rhs.b_ <= rhs.a_ * lhs.b_;
	}

	constexpr friend bool operator >=(const Rational& lhs, const Rational& rhs) {
		return rhs <= lhs;
	}

	friend std::ostream& operator << (std::ostream& out, const Rational& rational) {
		return out << rational.a_ << " " << rational.b_;
	}

	friend std::istream& operator >> (std::istream& in, Rational& rational) {
		in >> rational .a_ >> rational.b_;
		rational.normalize();
		return in;
	}

	void normalize() {
		const T g = gcd(a_, b_);
		a_ /= g;
		b_ /= g;
		if (b_ < 0) {
			a_ = -a_;
			b_ = -b_;
		}
	}

	void read_slash(std::istream& in) {
		char ch;
		in >> a_ >> ch >> b_;
		normalize();
	}

	void print_slash(std::ostream& out) const {
		out << a_ << "/" << b_;
	}

	constexpr Rational negate() const {
		return{ -a_, b_ };
	}

	constexpr Rational abs() const {
		return a_ < 0 ? Rational(-a_, b_) : Rational(a_, b_);
	}

	constexpr Rational inverse() const {
		return{ b_, a_ };
	}

	constexpr int sign() const {
		return a_ < 0 ? -1 : static_cast<int>(a_ > 0);
	}

	constexpr long double to_double() const {
		return static_cast<long double>(a_) / b_;
	}

	constexpr T to_int() const {
		return a_ / b_;
	}

	constexpr T floor() const {
		return to_int();
	}

	constexpr T ceil() const {
		return static_cast<T>(ceil(to_double() - EPS));
	}

	constexpr T round() const {
		const long double cur = to_double();
		return static_cast<T>(cur < 0 ? ceil(cur - 0.5 - EPS) : floor(cur + 0.5 + EPS));
	}

	constexpr long double frac() const {
		return to_double() - to_int();
	}

	constexpr Rational pow(const ll n) const {
		return{ binpow(a_, n), binpow(b_, n) };
	}

private:
	T a_;
	T b_;

};

namespace std {

template<typename T>
std::string to_string(const Rational<T>& arg) {
	return arg.to_string();
}

template<typename T>
struct hash<Rational<T>> {
	size_t operator()(const Rational<T>& arg) const {
		return hash<pair<T, T>>({ arg.a, arg.b });
	}
};

template<typename T>
void swap(Rational<T>& lhs, Rational<T>& rhs) {
	lhs.swap(rhs);
}

}
