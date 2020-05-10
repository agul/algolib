#pragma once
#include <numeric>
#include <vector>

template<typename T>
class Combination : public std::vector<T> {
public:
	Combination(const size_t n, const size_t k) : std::vector<T>(k), n_(n) {
		init();
	}

	void init() {
		std::iota(this->begin(), this->end(), 0);
	}

	bool next() {
		const size_t k = this->size();
		Combination tmp = *this;
		size_t index = k;
		do {
			--index;
			if (tmp[index] < n_ - k + index) {
				++tmp[index];
				for (size_t j = index + 1; j < k; ++j) {
					tmp[j] = tmp[j - 1] + 1;
				}
				this->swap(tmp);
				return true;
			}
		} while (index > 0);
		init();
		return false;
	}

private:
	size_t n_;
};