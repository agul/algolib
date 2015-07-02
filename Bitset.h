#pragma once
#include "Head.h"

#define bitset __MY_BITSET__

template<size_t N> class Bitset {
public:

	static const int BITSET_LENGTH = (N >> 5) + 1;
	unsigned int data[BITSET_LENGTH];

	Bitset() {
		fill_n(data, BITSET_LENGTH, 0);
	}

	Bitset& set(const int x) {
		data[x >> 5] |= (1 << (x & 31));
		return *this;
	}

	Bitset& flip(const int x) {
		data[x >> 5] ^= (1 << (x & 31));
		return *this;
	}

	Bitset& clear(const int x) {
		data[x >> 5] &= ~(1 << (x & 31));
		return *this;
	}

	bool get(const int x) const {
		return data[x >> 5] & (1 << (x & 31));
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
		fill_n(data, BITSET_LENGTH, 0);
		return *this;
	}

	Bitset& set() {
		fill_n(data, BITSET_LENGTH, 0xffffffff);
		return *this;
	}

	Bitset& flip() {
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			data[i] = ~data[i];
		}
		return *this;
	}

	int count() const {
		return accumulate(data, data + BITSET_LENGTH, 0, popcount);
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

};

#undef bitset