#pragma once
#include <string>
#include <utility>

#include "maths/mod_int.hpp"

template<typename T>
class DoubleHasher {
public:
	static const uint32_t AMOD = static_cast<uint32_t>(1e9) + 7;
	static const uint32_t BMOD = static_cast<uint32_t>(1e9) + 9;

	using a_integer_type = ModInt<T, AMOD>;
	using b_integer_type = ModInt<T, BMOD>;

	DoubleHasher() : DoubleHasher(0) {}
	DoubleHasher(const T x) : DoubleHasher(x, x) {}
	DoubleHasher(const T a, const T b) : DoubleHasher(static_cast<a_integer_type>(a), static_cast<b_integer_type>(b)) {}
	DoubleHasher(const a_integer_type a, const b_integer_type b) : a(a), b(b) {}

	DoubleHasher operator +(const DoubleHasher& rhs) const {
		return {a + rhs.a, b + rhs.b};
	}

	DoubleHasher operator -(const DoubleHasher& rhs) const {
		return {a - rhs.a, b - rhs.b};
	}

	DoubleHasher operator *(const int rhs) const {
		return {a * static_cast<a_integer_type>(rhs), b * static_cast<b_integer_type>(rhs)};
	}

	DoubleHasher operator *(const DoubleHasher& rhs) const {
		return {a * rhs.a, b * rhs.b};
	}

	bool operator ==(const DoubleHasher& rhs) const {
		return a == rhs.a && b == rhs.b;
	}

	bool operator <(const DoubleHasher& rhs) const {
		return a < rhs.a || (a == rhs.a && b < rhs.b);
	}

	explicit operator long long() const {
		return a.template as<int64_t>() * BMOD + b.template as<int64_t>() + 1;
	}

	std::string str() const {
		return to_string(std::make_pair(a, b));
	}

private:
	a_integer_type a;
	b_integer_type b;
};

template<typename T>
std::string to_string(const DoubleHasher<T>& value) {
	return value.str();
}
