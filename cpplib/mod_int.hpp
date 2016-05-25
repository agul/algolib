#pragma once
#include "Head.h"

template<typename T, T MOD = 1000000007>
class ModInt {
public:

	ModInt() : ModInt(0) {}

	template<typename U>
	ModInt(const U value) : value_(normalize(value)) {
		static_assert(mod > 0, "Modulo must be strictly positive.");
		// static_assert((std::equal<T, int32_t> && mod <= 0x3f3f3f3f) || (std::equal<T, int64_t> && mod <= 0x3f3f3f3f3f3f3f3fLL), "Modulo must be less than half of the max value for typename.");
	}

	constexpr operator T() const {
		return value_;
	}

	bool operator ==(const ModInt& rhs) const {
		return value_ == rhs.value_;
	}

	bool operator !=(const ModInt& rhs) const {
		return !operator==(rhs);
	}

	bool operator <(const ModInt& rhs) const {
		return value_ < rhs.value_;
	}

	ModInt operator +(const ModInt& rhs) const {
		T x = value_;
		return{ add_mod(x, rhs.value_, MOD) };
	}

	ModInt operator -(const ModInt& rhs) const {
		T x = value_;
		return{ sub_mod(x, rhs.value_, MOD) };
	}

private:
	T value_;

	template<typename U>
	T normalize(const U value) const {
		T ret = value % MOD;
		if (ret < 0) {
			return ret + MOD;
		}
		return ret;
	}

};