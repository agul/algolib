#pragma once
#include "geometry/line.hpp"
#include "geometry/point_vector.hpp"
#include "maths/decimal.hpp"
#include "maths/maths.hpp"

template<typename T, typename SquareT = T, typename DecimalT = Decimal<>>
class Circle {
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
	using line_type = Line<value_type, square_type, decimal_type>;
	/// caide keep
	using vector_type = Vector2D<value_type, square_type, decimal_type>;
	/// caide keep
	using decimal_point_type = typename point_type::decimal_point_type;
	/// caide keep
	using decimal_vector_type = typename vector_type::decimal_vector_type;
	 
	/// caide keep
	constexpr Circle() : Circle({ 0, 0 }, 0) {}
	constexpr Circle(const point_type& center, const value_type radius) : center_(center), radius_(radius) {}

	size_t intersect(const line_type& line, decimal_point_type& M, decimal_point_type& N) const {
		const decimal_type h = center_.dist(A, B);
		if (static_cast<decimal_type>(radius_) < h) {
			return 0;
		}
		const decimal_point_type height = center_.get_height(A, B);
		const decimal_type delta = std::sqrt(sqr<decimal_type>(radius_) - sqr(h));
		const decimal_vector_type vector = vector_type(A, B).normalize(delta);
		M = height.move_by(vector);
		N = height.move_by(-vector);
		if (vector.is_zero()) {
			return 1;
		}
		return 2;
	}

	constexpr const point_type& center() const {
		return center_;
	}

	constexpr value_type radius() const {
		return radius_;
	}

private:
	point_type center_;
	value_type radius_;
};