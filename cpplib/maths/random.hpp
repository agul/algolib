#pragma once
#include <chrono>
#include <random>

namespace Random {

static std::chrono::system_clock::rep current_timestamp() {
	return std::chrono::system_clock::now().time_since_epoch().count();
}

static std::chrono::system_clock::rep get_rand_seed() {
	return current_timestamp();
}

static std::mt19937_64 gen(get_rand_seed());
static std::uniform_int_distribution<long long> distrib(0, std::numeric_limits<long long>::max());

template<typename T>
static T get(const T r) {
	return distrib(gen) % r;
}

template<typename T>
static T get(const T l, const T r) {
	return get(r - l + 1) + l;
}

}