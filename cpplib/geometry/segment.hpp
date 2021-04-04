#pragma once
#include "geometry/line.hpp"
#include "geometry/point_vector.hpp"
#include "maths/decimal.hpp"
#include "maths/maths.hpp"

template<typename T, typename SquareT = T, typename DecimalT = Decimal<>>
class Segment {
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
    using line_type = Line<value_type, square_type, decimal_type>;

    Segment() : Segment({}, {}) {}
    Segment(const point_type& a, const point_type& b) : a_(a), b_(b) {}

    constexpr decimal_type length() const {
        return a_.dist(b_);
    }

    constexpr decimal_point_type center() const {
        return (a_ + b_).template as<decimal_point_type>() * 0.5;
    }

    constexpr const point_type& point_a() const {
        return a_;
    }

    constexpr const point_type& point_b() const {
        return b_;
    }

    constexpr bool operator ==(const Segment& rhs) const {
        return (a_ == rhs.a_ && b_ == rhs.b_) || (a_ == rhs.b_ && b_ == rhs.a_);
    }

    constexpr line_type to_line() const {
        return {a_, b_};
    }

    constexpr vector_type to_vector() const {
        return to_line().to_vector();
    }

    constexpr bool is_on_line(const line_type& line) const {
        return line == to_line();
    }

    constexpr bool is_parallel(const line_type& line) const {
        return to_line().is_parallel(line);
    }

    constexpr bool is_parallel(const Segment& segment) const {
        return is_parallel(segment.to_line());
    }

    constexpr bool starts_from(const point_type& point) const {
        return point == a_ || point == b_;
    }

    constexpr bool contains(const point_type& point, const bool strictly_inside = false) const {
        if (!strictly_inside && starts_from(point)) {
            return true;
        }
        const Vector2D<value_type, square_type, decimal_type> a_vector(point, a_);
        const Vector2D<value_type, square_type, decimal_type> b_vector(point, b_);
        return to_line().contains(point) && a_vector.scalar_mult(b_vector) < 0;
    }

private:
    point_type a_;
    point_type b_;
};
