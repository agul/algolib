#pragma once
#include <functional>
#include <vector>

template<typename T>
class SparseTableCmp {
public:
	using Comparator = std::function<bool(const T&, const T&)>;

	SparseTableCmp(const size_t size, const Comparator& less) :
		size_(size), less_(less), log_table_(size + 1, 0) {
		for (size_t i = 2; i <= size_; ++i) {
			log_table_[i] = log_table_[i >> 1] + 1;
		}
		data_.assign(log_table_[size_] + 1, std::vector<size_t>(size_));
	}

	void set_comparator(const Comparator& less) {
		less_ = less;
	}

	void build(const std::vector<T>& vec) {
		arr_ = &vec;
		for (size_t i = 0; i < size_; ++i) {
			data_[0][i] = i;
		}
		for (size_t level = 1, step = 2; step < size_; ++level, step <<= 1) {
			for (size_t i = 0; i + step <= size_; ++i) {
				const size_t x = data_[level - 1][i];
				const size_t y = data_[level - 1][i + (step >> 1)];
				data_[level][i] = (less_(arr_->at(x), arr_->at(y)) ? x : y);
			}
		}
	}

	size_t query(const size_t left, const size_t right) const {
		const size_t level = log_table_[right - left - 1];
		const size_t x = data_[level][left];
		const size_t y = data_[level][right - (1 << level)];
		return (less_(arr_->at(x), arr_->at(y)) ? x : y);
	}

private:
	std::vector<std::vector<size_t>> data_;
	std::vector<size_t> log_table_;
	Comparator less_;
	const std::vector<T>* arr_;
	size_t size_;
};