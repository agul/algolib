#pragma once
#include "Head.h"

#define bitset __MY_BITSET__
unordered_map<unsigned int, int> BITSET_COUNT;

template<size_t N> struct Bitset {

	static const int BITSET_LENGTH = (N >> 5) + 1;
	unsigned int data[BITSET_LENGTH];

	Bitset() {
		memset(data, 0, sizeof(data));
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
		memset(data, 0, sizeof(data));
		return *this;
	}

	Bitset& set() {
		memset(data, 0xff, sizeof(data));
		return *this;
	}

	Bitset& flip() {
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			data[i] = ~data[i];
		}
		return *this;
	}

	int count() const {
		int ret = 0, tmp, cur;
		for (int i = 0; i < BITSET_LENGTH; ++i) {
			if (BITSET_COUNT.count(tmp = data[i])) {
				ret += BITSET_COUNT[tmp];
			} else {
				cur = 0;
				while (tmp) {
					cur += tmp & 1;
					tmp >>= 1;
				}
				ret += (BITSET_COUNT[data[i]] = cur);
			}
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

};

#undef bitset