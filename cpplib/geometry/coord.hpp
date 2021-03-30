#pragma once
#include <functional>
#include <iostream>

#include "base/helpers.hpp"
#include "maths/decimal.hpp"
#include "maths/maths.hpp"

template<typename T, typename SquareT = T, typename DecimalT = Decimal<>>
class Coord2D {
public:
    /// caide keep
    using value_type = T;
    /// caide keep
    using square_type = SquareT;
    /// caide keep
    using decimal_type = DecimalT;
    /// caide keep
    using decimal_coord_type = Coord2D<decimal_type, decimal_type, decimal_type>;

    /// caide keep
    constexpr Coord2D() : Coord2D(0, 0) {}
    constexpr Coord2D(const value_type x, const value_type y) : x(x), y(y) {}

    decimal_type angle() const {
        return std::atan2(y, x);
    }

    constexpr Coord2D add(const Coord2D& rhs) const {
        return{ x + rhs.x, y + rhs.y };
    }

    constexpr Coord2D subtract(const Coord2D& rhs) const {
        return{ x - rhs.x, y - rhs.y };
    }

    constexpr Coord2D operator +(const Coord2D& rhs) const {
        return add(rhs);
    }

    constexpr Coord2D operator -(const Coord2D& rhs) const {
        return subtract(rhs);
    }

    Coord2D& operator +=(const Coord2D& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Coord2D& operator -=(const Coord2D& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    value_type& operator [](size_t index) {
        if (index == 0) {
            return x;
        }
        if (index == 1) {
            return y;
        }
    }

    constexpr size_t size() const {
        return 2;
    }

    const value_type& operator [](const size_t index) const {
        if (index == 0) {
            return x;
        }
        if (index == 1) {
            return y;
        }
    }

    bool operator == (const Coord2D& rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    bool operator < (const Coord2D& rhs) const {
        return x < rhs.x || (x == rhs.x && y < rhs.y);
    }

    /// caide keep
    template<typename C>
    C as() const {
        return{ static_cast<typename C::value_type>(x), static_cast<typename C::value_type>(y) };
    }

    std::string str() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    friend std::istream& operator >> (std::istream& in, Coord2D& rhs) {
        in >> rhs.x >> rhs.y;
        return in;
    }

    value_type x;
    value_type y;

protected:
    constexpr square_type squared_dist(const Coord2D& rhs) const {
        return sqr<square_type>(x - rhs.x) + sqr<square_type>(y - rhs.y);
    }

    decimal_type dist(const Coord2D& rhs) const {
        return std::sqrt(squared_dist(rhs));
    }

    template<typename Value = value_type, typename std::enable_if<std::is_integral<Value>::value>::type* = nullptr>
    constexpr value_type manhattan_dist(const Coord2D& rhs) const {
        return std::abs(x - rhs.x) + std::abs(y - rhs.y);
    }
};

template<typename T, typename SquareT = T, typename DecimalT = Decimal<>>
class Coord3D {
public:
    /// caide keep
    using value_type = T;
    /// caide keep
    using square_type = SquareT;
    /// caide keep
    using decimal_type = DecimalT;
    /// caide keep
    using decimal_coord_type = Coord3D<decimal_type, decimal_type, decimal_type>;

    /// caide keep
    constexpr Coord3D() : Coord3D(0, 0, 0) {}
    constexpr Coord3D(const value_type x, const value_type y, const value_type z) : x(x), y(y), z(z) {}

    constexpr Coord3D add(const Coord3D& rhs) const {
        return{ x + rhs.x, y + rhs.y, z + rhs.z };
    }

    constexpr Coord3D subtract(const Coord3D& rhs) const {
        return{ x - rhs.x, y - rhs.y, z - rhs.z };
    }

    constexpr Coord3D operator +(const Coord3D& rhs) const {
        return add(rhs);
    }

    constexpr Coord3D operator -(const Coord3D& rhs) const {
        return subtract(rhs);
    }

    Coord3D& operator +=(const Coord3D& rhs) const {
        x += rhs.x;
        y += rhs.y;
        y += rhs.z;
        return *this;
    }

    Coord3D& operator -=(const Coord3D& rhs) const {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    value_type& operator [](size_t index) {
        if (index == 0) {
            return x;
        }
        if (index == 1) {
            return y;
        }
        if (index == 2) {
            return z;
        }
    }

    const value_type& operator [](const size_t index) const {
        if (index == 0) {
            return x;
        }
        if (index == 1) {
            return y;
        }
        if (index == 2) {
            return z;
        }
    }

    constexpr size_t size() const {
        return 3;
    }

    /// caide keep
    template<typename C>
    C as() const {
        return{ static_cast<typename C::value_type>(x), static_cast<typename C::value_type>(y), static_cast<typename C::value_type>(z) };
    }

    std::string str() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }

    friend std::istream& operator >> (std::istream& in, Coord3D& rhs) {
        in >> rhs.x >> rhs.y >> rhs.z;
        return in;
    }

    value_type x;
    value_type y;
    value_type z;

protected:
    constexpr square_type squared_dist(const Coord3D& rhs) const {
        return sqr<square_type>(x - rhs.x) + sqr<square_type>(y - rhs.y) + sqr<square_type>(z - rhs.z);
    }

    decimal_type dist(const Coord3D& rhs) const {
        return std::sqrt(squared_dist(rhs));
    }

    template<typename std::enable_if<std::is_integral<value_type>::value>::type* = nullptr>
    constexpr value_type manhattan_dist(const Coord3D& rhs) const {
        return std::abs(x - rhs.x) + std::abs(y - rhs.y) + std::abs(z - rhs.z);
    }
};

template<typename Coord>
struct is_2d
{
    /// caide keep
    static constexpr bool value = std::is_same<Coord, Coord2D<typename Coord::value_type, typename Coord::square_type, typename Coord::decimal_type>>::value;
};

template<typename Coord>
struct is_3d
{
    /// caide keep
    static constexpr bool value = std::is_same<Coord, Coord3D<typename Coord::value_type, typename Coord::square_type, typename Coord::decimal_type>>::value;
};

template<typename Coord, typename std::enable_if<is_2d<Coord>::value>::type* = nullptr>
struct LessByY {
    bool operator()(const Coord& lhs, const Coord& rhs) const {
        return lhs.y < rhs.y || lhs.y == rhs.y && lhs.x < rhs.x;
    }
};

template<typename T, typename S, typename D>
std::string to_string(const Coord2D<T, S, D>& coord) {
    return coord.str();
}

template<typename T, typename S, typename D>
std::string to_string(const Coord3D<T, S, D>& coord) {
    return coord.str();
}

namespace std {

template<typename T, typename S, typename D>
struct hash<Coord2D<T, S, D>> {
    size_t operator()(const Coord2D<T, S, D>& arg) const {
        return hash<pair<T, T>>({ arg.x, arg.y });
    }
};

template<typename T, typename S, typename D>
struct hash<Coord3D<T, S, D>> {
    size_t operator()(const Coord3D<T, S, D>& arg) const {
        return arg.x * 877117 + arg.y * 1000000009 + arg.z;
    }
};

}  // namespace std
