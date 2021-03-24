#pragma once
#include <functional>
#include <iostream>
#include <type_traits>

#include "geometry/coord.hpp"
#include "maths/decimal.hpp"
#include "maths/maths.hpp"

template<typename Coord>
class Vector;

template<typename Coord>
class Point : public Coord {
public:
    /// caide keep
    using vector_type = Vector<Coord>;
    /// caide keep
    using value_type = typename Coord::value_type;
    /// caide keep
    using square_type = typename Coord::square_type;
    /// caide keep
    using decimal_type = typename Coord::decimal_type;
    /// caide keep
    using coordinates_type = Coord;

    /// caide keep
    using decimal_point_type = Point<typename Coord::decimal_coord_type>;
    /// caide keep
    using decimal_vector_type = typename vector_type::decimal_vector_type;

    /// caide keep
    constexpr Point() : Coord() {}

    /// caide keep
    template<typename C = Coord, typename std::enable_if<is_2d<C>::value>::type* = nullptr>
    constexpr Point(const value_type x, const value_type y) : Coord(x, y) {}

    /// caide keep
    template<typename C = Coord, typename std::enable_if<is_3d<C>::value>::type* = nullptr>
    constexpr Point(const value_type x, const value_type y, const value_type z) : Coord(x, y, z) {}

    explicit constexpr Point(const Coord& coord) : Coord(coord) {}

    constexpr Point add(const Point& rhs) const {
        return Point(Coord::add(rhs));
    }

    constexpr Point subtract(const Point& rhs) const {
        return Point(Coord::subtract(rhs));
    }

    constexpr Point operator +(const Point& rhs) const {
        return add(rhs);
    }

    constexpr Point operator -(const Point& rhs) const {
        return subtract(rhs);
    }

    Point& operator +=(const Point& rhs) {
        Coord::operator +=(rhs);
        return *this;
    }

    Point& operator -=(const Point& rhs) {
        Coord::operator -=(rhs);
        return *this;
    }

    constexpr square_type squared_dist(const Point& rhs) const {
        return Coord::squared_dist(rhs);
    }

    decimal_type dist(const Point& rhs) const {
        return Coord::dist(rhs);
    }

    template<typename std::enable_if<std::is_integral<value_type>::value>::type* = nullptr>
    constexpr value_type manhattan_dist(const Point& rhs) const {
        return Coord::manhattan_dist(rhs);
    }

    constexpr Point move_by(const vector_type& vector) const {
        return Point(this->x + vector.x, this->y + vector.y);
    }

    decimal_point_type get_height(const Point& A, const Point& B) const {
        const vector_type v(A, B);
        const vector_type u(A, *this);
        const decimal_type projection = decimal_type(v % u) / v.length();
        const auto projected = v.template as<decimal_vector_type>().normalize(projection);
        return A.template as<decimal_point_type>().move_by(projected);
    }

    decimal_type dist(const Point& A, const Point& B) const	{
        const decimal_type dist_ab = A.dist(B);
        const vector_type a(A, *this);
        const vector_type b(B, *this);
        const decimal_type vector_mult = a * b;
        return vector_mult.abs() / dist_ab;
    }

    /// caide keep
    template<typename P>
    constexpr P as() const {
        return P(Coord::template as<typename P::coordinates_type>());
    }
};

template<typename Coord>
class Vector : public Coord {
public:
    /// caide keep
    using point_type = Point<Coord>;
    /// caide keep
    using value_type = typename Coord::value_type;
    /// caide keep
    using square_type = typename Coord::square_type;
    /// caide keep
    using decimal_type = typename Coord::decimal_type;
    /// caide keep
    using coordinates_type = Coord;

    /// caide keep
    using decimal_vector_type = Vector<typename Coord::decimal_coord_type>;

    /// caide keep
    constexpr Vector() : Coord() {}

    /// caide keep
    template<typename C = Coord, typename std::enable_if<is_2d<C>::value>::type* = nullptr>
    constexpr Vector(const value_type x, const value_type y) : Coord(x, y) {}

    /// caide keep
    template<typename C = Coord, typename std::enable_if<is_3d<C>::value>::type* = nullptr>
    constexpr Vector(const value_type x, const value_type y, const value_type z) : Coord(x, y, z) {}

    constexpr explicit Vector(const Coord& coord) : Coord(coord) {}

    constexpr Vector(const point_type& a, const point_type& b) : Coord(b.x - a.x, b.y - a.y) {}

    constexpr Vector add(const Vector& rhs) const {
        return Vector(Coord::add(rhs));
    }

    constexpr Vector subtract(const Vector& rhs) const {
        return Vector(Coord::subtract(rhs));
    }

    constexpr Vector operator +(const Vector& rhs) const {
        return add(rhs);
    }

    constexpr Vector operator -(const Vector& rhs) const {
        return subtract(rhs);
    }

    Vector& operator +=(const Vector& rhs) const {
        Coord::operator +=(rhs);
        return *this;
    }

    Vector& operator -=(const Vector& rhs) const {
        Coord::operator -=(rhs);
        return *this;
    }

    constexpr Vector multiply(const value_type rhs) const {
        return Vector(this->x * rhs, this->y * rhs);
    }

    constexpr Vector operator *(const value_type rhs) const {
        return multiply(rhs);
    }

    decimal_type length() const {
        return Coord::dist(Coord());
    }

    constexpr bool is_zero() const {
        return length() == 0;
    }

    constexpr Vector negate() const {
        return Vector(-this->x, -this->y);
    }

    constexpr Vector operator -() const {
        return negate();
    }

    decimal_vector_type normalize(const decimal_type new_len = 1) const {
        const decimal_type len = length();
        if (len == 0) {
            if (new_len == 0) {
                return decimal_vector_type();
            }
            assert(false);  // non-zero vector size
        }
        return as<decimal_vector_type>().multiply(new_len / len);
    }

    constexpr value_type scalar_mult(const Vector& rhs) const {
        return this->x * rhs.x + this->y * rhs.y;
    }

    constexpr value_type vector_mult(const Vector& rhs) const {
        return this->x * rhs.y - this->y * rhs.x;
    }

    constexpr value_type operator %(const Vector& rhs) const {
        return scalar_mult(rhs);
    }

    constexpr value_type operator *(const Vector& rhs) const {
        return vector_mult(rhs);
    }

    /// caide keep
    template<typename V>
    constexpr V as() const {
        return V(Coord::template as<typename V::coordinates_type>());
    }
};

template<typename T, typename SquareT = T, typename DecimalT = Decimal<>>
using Point2D = Point<Coord2D<T, SquareT, DecimalT>>;

template<typename T, typename SquareT = T, typename DecimalT = Decimal<>>
using Point3D = Point<Coord3D<T, SquareT, DecimalT>>;

template<typename T, typename SquareT = T, typename DecimalT = Decimal<>>
using Vector2D = Vector<Coord2D<T, SquareT, DecimalT>>;

template<typename T, typename SquareT = T, typename DecimalT = Decimal<>>
using Vector3D = Vector<Coord3D<T, SquareT, DecimalT>>;

template<typename Coord>
struct is_2d<Point<Coord>>
{
    /// caide keep
    static constexpr bool value = is_2d<Coord>::value;
};

template<typename Coord>
struct is_3d<Point<Coord>>
{
    /// caide keep
    static constexpr bool value = is_3d<Coord>::value;
};

template<typename Coord>
struct is_2d<Vector<Coord>>
{
    /// caide keep
    static constexpr bool value = is_2d<Coord>::value;
};

template<typename Coord>
struct is_3d<Vector<Coord>>
{
    /// caide keep
    static constexpr bool value = is_3d<Coord>::value;
};

namespace std {

template<typename Coord>
struct hash<Point<Coord>> {
    size_t operator()(const Point<Coord>& arg) const {
        return hash<Coord>(arg);
    }
};

template<typename Coord>
struct hash<Vector<Coord>> {
    size_t operator()(const Vector<Coord>& arg) const {
        return hash<Coord>(arg);
    }
};

}
