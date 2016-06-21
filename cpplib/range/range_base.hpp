#pragma once
#include <utility>

template<typename T>
class Range {
public:
	constexpr Range(T&& begin, T&& end) : begin_(std::move(begin)), end_(std::move(end)) {}

	constexpr T begin() const {
		return begin_;
	}

	constexpr T end() const {
		return end_;
	}

private:
	T begin_;
	T end_;

};

template<typename T>
Range<T> make_range(T&& begin, T&& end) {
	return Range<T>(std::move(begin), std::move(end));
}
