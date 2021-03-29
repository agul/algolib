#pragma once
#include <type_traits>

#include "geometry/point_vector.hpp"
#include "maths/decimal.hpp"
#include "maths/maths.hpp"

template<typename T, typename SquareT = T, typename DecimalT = Decimal<>>
class Line {
public:
	/// caide keep
	using value_type = T;
	/// caide keep
	using square_type = SquareT;
	/// caide keep
	using decimal_type = DecimalT;
	/// caide keep
	using point_type = Point2D<value_type, square_type, decimal_type>;
	/// caide keep
	using vector_type = Vector2D<value_type, square_type, decimal_type>;
	/// caide keep
	using decimal_point_type = typename point_type::decimal_point_type;
	/// caide keep
	using decimal_vector_type = typename vector_type::decimal_vector_type;

	/// caide keep
	constexpr Line() : Line(0, 0, 0) {}

	constexpr Line(const value_type a, const value_type b, const value_type c) : a_(a), b_(b), c_(c) {}

	constexpr Line(const point_type& a, const point_type& b)  {
		a_ = a.y - b.y;
		b_ = b.x - a.x;
		c_ = -a_ * a.x - b_ * a.y;
	}

	constexpr vector_type to_vector() const {
		return vector_type(b, -a);
	}

	constexpr decimal_point_type point_a() const {
	    if (is_equal_to_zero(b_)) {
            constexpr const value_type x = static_cast<decimal_type>(-c_) / a_;
	        return decimal_point_type(x, 0);
	    }
		return decimal_point_type(0, static_cast<decimal_type>(-c_) / b_);
	}

	constexpr decimal_point_type point_b() const {
        if (is_equal_to_zero(b_)) {
            constexpr const value_type x = static_cast<decimal_type>(-c_) / a_;
            return decimal_point_type(x, 100);
        }
		constexpr const value_type x = 100;
		return decimal_point_type(x, static_cast<decimal_type>(-c_ - a_ * x) / b_);
	}

	void normalize() {
		if (std::is_integral<value_type>::value) {
			if (a_ < 0) {
				a_ = -a_;
				b_ = -b_;
				c_ = -c_;
			}
			const value_type g = gcd(a_, gcd(abs(b_), abs(c_)));
			a_ /= g;
			b_ /= g;
			c_ /= g;
			return;
		}
		const value_type z = std::sqrt(sqr(a_) + sqr(b_));
		if (!is_equal_to_zero(z)) {
			a_ /= z;
			b_ /= z;
			c_ /= z;
		}
	}

	constexpr value_type value(const point_type& point) const {
	    return point.x * a_ + point.y * b_ + c_;
	}

	decimal_type angle() const {
	    return to_vector().angle();
	}

	constexpr bool contains(const point_type& point) const {
	    return is_equal_to_zero(value(point));
	}

	constexpr Line perpendicular(const point_type& point) const {
	    return Line{-b_, a_, b_ * point.x - a_ * point.y};
	}

	constexpr bool is_parallel(const Line& rhs) const {
        return is_equal_to_zero(a_ * rhs.b_ - b_ * rhs.a_);
	}

	constexpr decimal_type dist(const point_type& point) const {
	    return std::abs(value(point));
	}

	constexpr bool operator ==(const Line& rhs) const {
	    return is_parallel(rhs) && is_equal_to_zero(a_ * rhs.c_ - c_ * rhs.a_) && is_equal_to_zero(b_ * rhs.c_ - c_ * rhs.b_);
	}

private:
	value_type a_;
	value_type b_;
	value_type c_;
};
