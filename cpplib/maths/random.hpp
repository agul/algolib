#pragma once
#include <chrono>
#include <limits>
#include <random>

namespace Random {

static std::chrono::system_clock::rep current_timestamp() {
	return std::chrono::system_clock::now().time_since_epoch().count();
}

static uint64_t get_rand_seed() {
	return static_cast<uint64_t>(current_timestamp());
}

static std::mt19937_64 engine(get_rand_seed());
static std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());

template<typename T>
static T get(const T right) {
	return distribution(engine) % right;
}

template<typename T>
static T get(const T left, const T right) {
	return get(right - left + 1) + left;
}

}  // namespace Random
