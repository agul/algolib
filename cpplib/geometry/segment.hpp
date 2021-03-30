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
        decimal_point_type center = a_ + b_;
        for (size_t i = 0; i < center.size(); ++i) {
            center[i] /= 2;
        }
        return center;
    }

    constexpr const point_type& point_a() const {
        return a_;
    }

    constexpr const point_type& point_b() const {
        return b_;
    }

    constexpr bool operator==(const Segment& rhs) const {
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

private:
    point_type a_;
    point_type b_;
};
