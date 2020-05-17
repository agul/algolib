#pragma once
#include <functional>
#include <utility>

namespace std {

#ifndef STD_HASH_PAIR
template<typename T, typename U>
struct hash<std::pair<T, U>> {
	std::size_t operator()(const std::pair<T, U>& pair) const {
		return pair.first * 877117 + pair.second;
	}
};
#endif

}  // namespace std
