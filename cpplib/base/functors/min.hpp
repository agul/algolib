#pragma once
#include <algorithm>

struct Min {
	template<typename T>
	T operator()(const T& lhs, const T& rhs) const {
		return std::min(lhs, rhs);
	}
};
