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
		return atan2(y, x);
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

	Coord2D& operator +=(const Coord2D& rhs) const {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Coord2D& operator -=(const Coord2D& rhs) const {
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

	/// caide keep
	template<typename C>
	C as() const {
		return{ static_cast<typename C::value_type>(x), static_cast<typename C::value_type>(y), static_cast<typename C::value_type>(z) };
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

}
