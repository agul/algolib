#pragma once
#include <vector>

#include "base/helpers.hpp"
#include "maths/maths.hpp"

struct maximal_element_search_tag {};
struct prime_modulo_calculations_tag {};

template<typename T>
class Matrix {
public:
	using RowStorage = std::vector<T>;
	using DataStorage = std::vector<RowStorage>;

	Matrix(const size_t rows_cnt, const size_t cols_cnt, const T mod = 1000000007) :
		rows_cnt_(rows_cnt), cols_cnt_(cols_cnt),
		data_(rows_cnt, RowStorage(cols_cnt, 0)),
		mod_(mod) {}

	Matrix() = delete;

	template<typename U>
	Matrix(const Matrix<U>& matrix) : rows_cnt_(matrix.rows_cnt()), cols_cnt_(matrix.cols_cnt()),
		data_(matrix.rows_cnt(), RowStorage(matrix.cols_cnt())) {
		for (size_t i = 0; i < rows_cnt_; ++i) {
			for (size_t j = 0; j < cols_cnt_; ++j) {
				data_[i][j] = static_cast<T>(matrix[i][j]);
			}
		}
	}

	size_t get_matrix_rank() const;
	size_t get_matrix_rank(maximal_element_search_tag) const;
	size_t get_matrix_rank(prime_modulo_calculations_tag, const T& mod) const;

	void swap_rows(const size_t i, const size_t j);
	void swap_cols(const size_t i, const size_t j);

	void shuffle();
	void shuffle_cols();
	void shuffle_rows();

	typename DataStorage::iterator begin() {
		return data_.begin();
	}

	typename DataStorage::const_iterator begin() const {
		return data_.begin();
	}

	typename DataStorage::iterator end() {
		return data_.end();
	}

	typename DataStorage::const_iterator end() const {
		return data_.end();
	}

	RowStorage get_column(const size_t index) const {
		RowStorage column;
		column.reserve(rows_cnt_);
		for (const auto& row : data_) {
			column.emplace_back(row[index]);
		}
		return column;
	}

	RowStorage& operator[] (const size_t index) {
		return data_[index];
	}

	const RowStorage& operator[] (const size_t index) const {
		return data_[index];
	}

	size_t rows_cnt() const {
		return rows_cnt_;
	}

	size_t cols_cnt() const {
		return cols_cnt_;
	}

	Matrix operator *(const Matrix& rhs) const {
		Matrix result(rows_cnt_, rhs.cols_cnt_);
		for (size_t i = 0; i < rows_cnt_; i++) {
			for (size_t k = 0; k < rhs.cols_cnt_; k++) {
				for (size_t j = 0; j < rhs.rows_cnt_; j++) {
					result[i][k] += data_[i][j] * rhs[j][k];
				}
			}
		}
		return result;
	}

	Matrix& operator *=(const Matrix& rhs) {
		Matrix result = operator *(rhs);
		swap(result);
		return *this;
	}

	template<typename U>
	Matrix binpow(U b) const {
		static_assert(std::is_integral<U>::value, "Degree must be integral. For real degree use pow.");
		Matrix ret = identity_matrix(rows_cnt_, cols_cnt_);
		Matrix a = *this;
		while (b != 0) {
			if ((b & 1) != 0) {
				ret *= a;
			}
			a *= a;
			b >>= 1;
		}
		return ret;
	}

	void swap(Matrix& rhs) {
		data_.swap(rhs.data_);
		std::swap(mod_, rhs.mod_);
		std::swap(rows_cnt_, rhs.rows_cnt_);
		std::swap(cols_cnt_, rhs.cols_cnt_);
	}

	static Matrix identity_matrix(const size_t rows_cnt, const size_t cols_cnt) {
		Matrix result(rows_cnt, cols_cnt);
		for (size_t i = 0; i < std::min(rows_cnt, cols_cnt); ++i) {
			result[i][i] = 1;
		}
		return result;
	}

private:
	size_t rows_cnt_;
	size_t cols_cnt_;
	T mod_;

	DataStorage data_;
};

template <typename T>
size_t Matrix<T>::get_matrix_rank() const {
	Matrix<long double> tmp(*this);
	std::vector<bool> used(rows_cnt_, false);
	size_t rank = cols_cnt_;
	for (size_t col = 0; col < cols_cnt_; ++col) {
		size_t best_row = std::numeric_limits<size_t>::max();
		for (size_t row = 0; row < rows_cnt_; ++row) {
			if (!used[row] && !is_equal_to_zero(tmp[row][col])) {
				best_row = row;
				break;
			}
		}
		if (best_row == std::numeric_limits<size_t>::max()) {
			--rank;
			continue;
		}
		used[best_row] = true;
		for (size_t j = col + 1; j < cols_cnt_; ++j) {
			tmp[best_row][j] /= tmp[best_row][col];
		}
		for (size_t row = 0; row < rows_cnt_; ++row) {
			if (used[row] || is_equal_to_zero(tmp[row][col])) {
				continue;
			}
			for (size_t j = col + 1; j < cols_cnt_; ++j) {
				tmp[row][j] -= tmp[best_row][j] * tmp[row][col];
			}
		}
	}
	return rank;
}

template <typename T>
size_t Matrix<T>::get_matrix_rank(maximal_element_search_tag) const {
	Matrix<long double> tmp(*this);
	std::vector<bool> used(rows_cnt_, false);
	size_t rank = 0;
	for (size_t col = 0; col < cols_cnt_; ++col) {
		T max_value = EPS;
		size_t best_row = std::numeric_limits<size_t>::max();
		for (size_t row = 0; row < rows_cnt_; ++row) {
			if (!used[row] && umax(max_value, abs(tmp[row][col]))) {
				best_row = row;
			}
		}
		if (best_row == std::numeric_limits<size_t>::max()) {
			continue;
		}
		++rank;
		used[best_row] = true;
		for (size_t j = col + 1; j < cols_cnt_; ++j) {
			tmp[best_row][j] /= tmp[best_row][col];
		}
		for (size_t row = 0; row < rows_cnt_; ++row) {
			if (used[row] || is_equal_to_zero(tmp[row][col])) {
				continue;
			}
			for (size_t j = col + 1; j < cols_cnt_; ++j) {
				tmp[row][j] -= tmp[best_row][j] * tmp[row][col];
			}
		}
	}
	return rank;
}

template <typename T>
size_t Matrix<T>::get_matrix_rank(prime_modulo_calculations_tag, const T& mod) const {
	static_assert(std::is_integral<T>::value, "Integral type required to process calculations by prime modulo");
	Matrix<long long> tmp(*this);
	for (auto& row : tmp) {
		for (auto& col : row) {
			if (col < 0) {
				col += mod;
			}
		}
	}
	std::vector<bool> used(rows_cnt_, false);
	size_t rank = cols_cnt_;
	for (size_t col = 0; col < cols_cnt_; ++col) {
		size_t best_row = std::numeric_limits<size_t>::max();
		for (size_t row = 0; row < rows_cnt_; ++row) {
			if (!used[row] && !is_equal_to_zero(tmp[row][col])) {
				best_row = row;
				break;
			}
		}
		if (best_row == std::numeric_limits<size_t>::max()) {
			--rank;
			continue;
		}
		used[best_row] = true; 
		for (size_t j = col + 1; j < cols_cnt_; ++j) {
			mul_mod(tmp[best_row][j], inverseElement(tmp[best_row][col], mod), mod);
		}
		for (size_t row = 0; row < rows_cnt_; ++row) {
			if (used[row] || is_equal_to_zero(tmp[row][col])) {
				continue;
			}
			for (size_t j = col + 1; j < cols_cnt_; ++j) {
				long long x = tmp[best_row][j];
				mul_mod(x, tmp[row][col], mod);
				sub_mod(tmp[row][j], x, mod);
			}
		}
	}
	return rank;
}

template<typename T>
void Matrix<T>::swap_rows(const size_t i, const size_t j) {
	data_[i].swap(data_[j]);
}

template<typename T>
void Matrix<T>::swap_cols(const size_t i, const size_t j) {
	for (auto& row : data_) {
		std::swap(row[i], row[j]);
	}
}

template<typename T>
void Matrix<T>::shuffle_rows() {
	DataStorage tmp;
	std::vector<size_t> indices(rows_cnt_);
	for (size_t i = 0; i < rows_cnt_; ++i) {
		indices[i] = i;
	}
	std::random_shuffle(indices.begin(), indices.end());
	for (const auto& index : indices) {
		tmp.emplace_back(data_[index]);
	}
	data_.swap(tmp);
}

template<typename T>
void Matrix<T>::shuffle_cols() {
	DataStorage tmp(rows_cnt_, cols_cnt_);
	std::vector<size_t> indices(cols_cnt_);
	for (size_t i = 0; i < cols_cnt_; ++i) {
		indices[i] = i;
	}
	std::random_shuffle(indices.begin(), indices.end());
	for (size_t j = 0; j < cols_cnt_; ++j) {
		size_t index = indices[j];
		for (size_t row = 0; row < rows_cnt_; ++row) {
			tmp[row][j] = data_[row][index];
		}
	}
	data_.swap(tmp);
}

template<typename T>
void Matrix<T>::shuffle() {
	shuffle_rows();
	shuffle_cols();
}
