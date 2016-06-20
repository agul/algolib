#pragma once
#include <algorithm>
#include <numeric>
#include <vector>

template<typename T>
class Permutation : public std::vector<T> {
public:
	Permutation() : Permutation(0) {}

	explicit Permutation(const size_t size) : std::vector<T>(size) {
		init();
	}

	template<typename I>
	Permutation(const I first, const I last) : std::vector<T>(first, last) {}

	Permutation(std::initializer_list<T>&& list) : std::vector<T>(std::move(list)) {}

	void init() {
		std::iota(this->begin(), this->end(), 0);
	}

	Permutation multiply(const Permutation& rhs) const {
		Permutation res(this->size());
		for (size_t i = 0; i < this->size(); ++i) {
			res[i] = this->at(rhs[i]);
		}
		return res;
	}

	Permutation operator *(const Permutation& rhs) const {
		return multiply(rhs);
	}

	Permutation& operator *=(const Permutation& rhs) {
		Permutation res = multiply(rhs);
		this->swap(res);
		return *this;
	}

	Permutation pow(const ll deg) const {
		Permutation result(this->size());
		Permutation a = *this;
		ll b = deg;
		while (b > 0) {
			if ((b & 1) > 0) {
				result *= a;
			}
			a *= a;
			b >>= 1;
		}
		return result;
	}

	bool next() {
		return std::next_permutation(this->begin(), this->end());
	}

};
