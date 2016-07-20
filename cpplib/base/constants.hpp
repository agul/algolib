#pragma once

static constexpr long double PI = 3.14159265358979323846;
static constexpr int INF = 0x3f3f3f3f;
static constexpr long double EPS = 1e-9;
static constexpr long long LINF = 0x3f3f3f3f3f3f3f3fLL;

static constexpr int DX[8] = { -1, 0, 1, 0, -1, -1, 1, 1 };
static constexpr int DY[8] = { 0, 1, 0, -1, -1, 1, 1, -1 };

static constexpr size_t kDayMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

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
