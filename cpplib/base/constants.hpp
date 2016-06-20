#pragma once

constexpr const long double PI = 3.14159265358979323846;
constexpr const int INF = 0x3f3f3f3f;
constexpr const long double EPS = 1e-9;
constexpr const long long LINF = 0x3f3f3f3f3f3f3f3fLL;

constexpr const int DX[] = { -1, 0, 1, 0, -1, -1, 1, 1 };
constexpr const int DY[] = { 0, 1, 0, -1, -1, 1, 1, -1 };

enum class Direction {
	Up,
	Right,
	Down,
	Left,
	UpLeft,
	UpRight,
	DownRight,
	DownLeft,
	North = Up,
	East = Right,
	South = Down,
	West = Left,
	NorthWest = UpLeft,
	NorthEast = UpRight,
	SouthEast = DownRight,
	SouthWest = DownLeft
};
