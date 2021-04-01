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
    using coordinates_type = Coord;
    /// caide keep
    using vector_type = Vector<coordinates_type>;
    /// caide keep
    using value_type = typename coordinates_type::value_type;
    /// caide keep
    using square_type = typename coordinates_type::square_type;
    /// caide keep
    using decimal_type = typename coordinates_type::decimal_type;

    /// caide keep
    using decimal_point_type = Point<typename coordinates_type::decimal_coord_type>;
    /// caide keep
    using decimal_vector_type = typename vector_type::decimal_vector_type;

    /// caide keep
    constexpr Point() : coordinates_type() {}

    /// caide keep
    template<typename C = coordinates_type, typename std::enable_if<is_2d<C>::value>::type* = nullptr>
    constexpr Point(const value_type x, const value_type y) : coordinates_type(x, y) {}

    /// caide keep
    template<typename C = coordinates_type, typename std::enable_if<is_3d<C>::value>::type* = nullptr>
    constexpr Point(const value_type x, const value_type y, const value_type z) : coordinates_type(x, y, z) {}

    explicit constexpr Point(const coordinates_type& coord) : coordinates_type(coord) {}

    constexpr Point add(const Point& rhs) const {
        return Point{coordinates_type::add(rhs)};
    }

    constexpr Point subtract(const Point& rhs) const {
        return Point{coordinates_type::subtract(rhs)};
    }

    constexpr Point multiply(const value_type rhs) const {
        return Point{coordinates_type::multiply(rhs)};
    }

    constexpr Point operator +(const Point& rhs) const {
        return add(rhs);
    }

    constexpr Point operator -(const Point& rhs) const {
        return subtract(rhs);
    }

    constexpr Point operator *(const value_type rhs) const {
        return multiply(rhs);
    }

    Point& operator +=(const Point& rhs) {
        coordinates_type::operator +=(rhs);
        return *this;
    }

    Point& operator -=(const Point& rhs) {
        coordinates_type::operator -=(rhs);
        return *this;
    }

    Point& operator *=(const value_type rhs) {
        coordinates_type::operator *=(rhs);
        return *this;
    }

    constexpr square_type squared_dist(const Point& rhs) const {
        return coordinates_type::squared_dist(rhs);
    }

    decimal_type dist(const Point& rhs) const {
        return coordinates_type::dist(rhs);
    }

    template<typename Value = value_type, typename std::enable_if<std::is_integral<Value>::value>::type* = nullptr>
    constexpr value_type manhattan_dist(const Point& rhs) const {
        return coordinates_type::manhattan_dist(rhs);
    }

    constexpr Point move_by(const vector_type& vector) const {
        return {this->x + vector.x, this->y + vector.y};
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
        return P{coordinates_type::template as<typename P::coordinates_type>()};
    }
};

template<typename Coord>
class Vector : public Coord {
public:
    /// caide keep
    using coordinates_type = Coord;
    /// caide keep
    using point_type = Point<coordinates_type>;
    /// caide keep
    using value_type = typename coordinates_type::value_type;
    /// caide keep
    using square_type = typename coordinates_type::square_type;
    /// caide keep
    using decimal_type = typename coordinates_type::decimal_type;
    /// caide keep
    using decimal_vector_type = Vector<typename coordinates_type::decimal_coord_type>;
    /// caide keep
    using squared_vector_type = Vector<typename coordinates_type::squared_coord_type>;

    /// caide keep
    constexpr Vector() : coordinates_type() {}

    /// caide keep
    template<typename C = coordinates_type, typename std::enable_if<is_2d<C>::value>::type* = nullptr>
    constexpr Vector(const value_type x, const value_type y) : coordinates_type(x, y) {}

    /// caide keep
    template<typename C = coordinates_type, typename std::enable_if<is_3d<C>::value>::type* = nullptr>
    constexpr Vector(const value_type x, const value_type y, const value_type z) : coordinates_type(x, y, z) {}

    constexpr explicit Vector(const coordinates_type& coord) : coordinates_type(coord) {}

    constexpr Vector(const point_type& a, const point_type& b) : coordinates_type(b.x - a.x, b.y - a.y) {}

    constexpr Vector add(const Vector& rhs) const {
        return Vector{coordinates_type::add(rhs)};
    }

    constexpr Vector subtract(const Vector& rhs) const {
        return Vector{coordinates_type::subtract(rhs)};
    }

    constexpr Vector multiply(const value_type rhs) const {
        return Vector{coordinates_type::multiply(rhs)};
    }

    constexpr Vector operator +(const Vector& rhs) const {
        return add(rhs);
    }

    constexpr Vector operator -(const Vector& rhs) const {
        return subtract(rhs);
    }

    constexpr Vector operator *(const value_type rhs) const {
        return multiply(rhs);
    }

    Vector& operator +=(const Vector& rhs) const {
        coordinates_type::operator +=(rhs);
        return *this;
    }

    Vector& operator -=(const Vector& rhs) const {
        coordinates_type::operator -=(rhs);
        return *this;
    }

    Vector& operator *=(const value_type rhs) const {
        coordinates_type::operator *=(rhs);
        return *this;
    }

    decimal_type length() const {
        return coordinates_type::dist({});
    }

    constexpr bool is_zero() const {
        return length() == 0;
    }

    constexpr Vector negate() const {
        return multiply(-1);
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

    template<typename C = coordinates_type, typename std::enable_if<is_2d<C>::value>::type* = nullptr>
    constexpr square_type scalar_mult(const Vector& rhs) const {
        return static_cast<square_type>(this->x) * rhs.x + static_cast<square_type>(this->y) * rhs.y;
    }

    template<typename C = coordinates_type, typename std::enable_if<is_3d<C>::value>::type* = nullptr>
    constexpr square_type scalar_mult(const Vector& rhs) const {
        return static_cast<square_type>(this->x) * rhs.x + static_cast<square_type>(this->y) * rhs.y + static_cast<square_type>(this->z) * rhs.z;
    }

    template<typename C = coordinates_type, typename std::enable_if<is_2d<C>::value>::type* = nullptr>
    constexpr square_type vector_mult(const Vector& rhs) const {
        return static_cast<square_type>(this->x) * rhs.y - static_cast<square_type>(this->y) * rhs.x;
    }

    template<typename C = coordinates_type, typename std::enable_if<is_3d<C>::value>::type* = nullptr>
    constexpr squared_vector_type vector_mult(const Vector& rhs) const {
        return {
            static_cast<square_type>(this->y) * rhs.z - static_cast<square_type>(this->z) * rhs.y,
            static_cast<square_type>(this->z) * rhs.x - static_cast<square_type>(this->x) * rhs.z,
            static_cast<square_type>(this->x) * rhs.y - static_cast<square_type>(this->y) * rhs.x,
        };
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
        return V{coordinates_type::template as<typename V::coordinates_type>()};
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

}  // namespace std
