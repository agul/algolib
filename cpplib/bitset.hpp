#pragma once
#include <algorithm>

template<size_t N>
class Bitset {
public:
	static const int BITSET_LENGTH = (N >> 6) + 1;
	unsigned long long data[BITSET_LENGTH];

	Bitset() {
		std::fill_n(data, BITSET_LENGTH, 0);
	}

	Bitset& set(const int x) {
		data[x >> 6] |= (1LL << (x & 63));
		return *this;
	}

	Bitset& flip(const int x) {
		data[x >> 6] ^= (1LL << (x & 63));
		return *this;
	}

	Bitset& clear(const int x) {
		data[x >> 6] &= ~(1LL << (x & 63));
		return *this;
	}

	bool get(const int x) const {
		return data[x >> 6] & (1LL << (x & 63));
	}

	int size() const {
		return N;
	}

	bool any() const {
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			if (data[i]) {
				return true;
			}
		}
		return false;
	}

	bool none() const {
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			if (data[i]) {
				return false;
			}
		}
		return true;
	}

	Bitset& reset() {
		std::fill_n(data, BITSET_LENGTH, 0);
		return *this;
	}

	Bitset& set() {
		std::fill_n(data, BITSET_LENGTH, 0xffffffffffffffffLL);
		return *this;
	}

	Bitset& flip() {
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			data[i] = ~data[i];
		}
		return *this;
	}

	int count() const {
		int ret = 0;
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			ret += popcount(data[i]);
		}
		return ret;
	}

	Bitset& operator &= (const Bitset& bitset) {
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			data[i] &= bitset.data[i];
		}
		return *this;
	}

	Bitset& operator |= (const Bitset& bitset) {
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			data[i] |= bitset.data[i];
		}
		return *this;
	}

	Bitset& operator ^= (const Bitset& bitset) {
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			data[i] ^= bitset.data[i];
		}
		return *this;
	}

	Bitset operator ~ () const {
		Bitset ret;
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			ret.data[i] = ~data[i];
		}
		return ret;
	}

	bool operator == (const Bitset& bitset) const {
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			if (data[i] != bitset.data[i]) {
				return false;
			}
		}
		return true;
	}

	bool operator != (const Bitset& bitset) const {
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			if (data[i] != bitset.data[i]) {
				return true;
			}
		}
		return false;
	}

	Bitset operator & (const Bitset& bitset) const {
		Bitset ret;
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			ret.data[i] = data[i] & bitset.data[i];
		}
		return ret;
	}

	Bitset operator | (const Bitset& bitset) const {
		Bitset ret;
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			ret.data[i] = data[i] | bitset.data[i];
		}
		return ret;
	}

	Bitset operator ^ (const Bitset& bitset) const {
		Bitset ret;
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			ret.data[i] = data[i] ^ bitset.data[i];
		}
		return ret;
	}

	Bitset rotate_left(const size_t shift) const {
		Bitset ret;
		int full_shift = shift >> 6;
		int rem_shift = shift & 63;
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			int ind = i + full_shift;
			if (ind >= BITSET_LENGTH) {
				ind -= BITSET_LENGTH;
			}
			int prv = i - 1;
			if (prv < 0) {
				prv += BITSET_LENGTH;
			}
			ret.data[ind] = (data[i] << rem_shift) | (data[prv] >> (64 - rem_shift));
		}
		return ret;
	}

};