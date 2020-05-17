#pragma once
#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>

#include "base/helpers.hpp"
#include "is_equal_to_zero.hpp"
#include "maths.hpp"
#include "random.hpp"

struct maximal_element_search_tag {};
struct prime_modulo_calculations_tag {};

template<typename T>
class Matrix {
public:
	using value_type = T;
	using size_type = std::size_t;

	using vector_container_type = std::vector<value_type>;
	using matrix_container_type = std::vector<vector_container_type>;

	Matrix(const size_type rows_cnt, const size_type cols_cnt, const value_type mod = 1000000007) :
			rows_cnt_(rows_cnt),
			cols_cnt_(cols_cnt),
			data_(rows_cnt, vector_container_type(cols_cnt, 0)),
			mod_(mod)
	{}

	Matrix(const std::initializer_list<std::initializer_list<value_type>>& initializer_list, const value_type mod = 1000000007) :
			rows_cnt_(initializer_list.size()),
			cols_cnt_(0),
			mod_(mod)
	{
		data_.reserve(initializer_list.size());
		for (const auto& row : initializer_list) {
			if (data_.empty()) {
				cols_cnt_ = row.size();
			} else {
				if (cols_cnt_ != row.size()) {
					throw std::out_of_range("Matrix<T> initializer list must have rows of the same size");
				}
			}
			data_.emplace_back(row);
		}
	}

	template<typename U>
	explicit Matrix(const Matrix<U>& matrix) :
			rows_cnt_(matrix.rows_cnt()),
			cols_cnt_(matrix.cols_cnt()),
			data_(matrix.rows_cnt(), vector_container_type(matrix.cols_cnt()))
	{
		for (size_type i = 0; i < rows_cnt_; ++i) {
			for (size_type j = 0; j < cols_cnt_; ++j) {
				data_[i][j] = static_cast<value_type>(matrix[i][j]);
			}
		}
	}

	size_type get_matrix_rank() const;
	size_type get_matrix_rank(maximal_element_search_tag) const;
	size_type get_matrix_rank(prime_modulo_calculations_tag, const value_type& mod) const;

	void swap_rows(const size_type i, const size_type j) {
		data_[i].swap(data_[j]);
	}

	void swap_cols(const size_type i, const size_type j) {
		for (auto& row : data_) {
			std::swap(row[i], row[j]);
		}
	}

	void shuffle() {
		shuffle_rows();
		shuffle_cols();
	}

	void shuffle_cols() {
		matrix_container_type tmp(rows_cnt_, cols_cnt_);
		std::vector<size_type> indices(cols_cnt_);
		for (size_type i = 0; i < cols_cnt_; ++i) {
			indices[i] = i;
		}
		std::shuffle(indices.begin(), indices.end(), Random::random_engine());
		for (size_type j = 0; j < cols_cnt_; ++j) {
			size_type index = indices[j];
			for (size_type row = 0; row < rows_cnt_; ++row) {
				tmp[row][j] = data_[row][index];
			}
		}
		data_.swap(tmp);
	}

	void shuffle_rows() {
		matrix_container_type tmp;
		tmp.reserve(rows_cnt_);
		std::vector<size_type> indices(rows_cnt_);
		for (size_type i = 0; i < rows_cnt_; ++i) {
			indices[i] = i;
		}
		std::shuffle(indices.begin(), indices.end(), Random::random_engine());
		for (const auto& index : indices) {
			tmp.emplace_back(data_[index]);
		}
		data_.swap(tmp);
	}

	typename matrix_container_type::iterator begin() {
		return data_.begin();
	}

	typename matrix_container_type::const_iterator begin() const {
		return data_.begin();
	}

	typename matrix_container_type::iterator end() {
		return data_.end();
	}

	typename matrix_container_type::const_iterator end() const {
		return data_.end();
	}

	vector_container_type get_column(const size_type index) const {
		vector_container_type column;
		column.reserve(rows_cnt_);
		for (const auto& row : data_) {
			column.emplace_back(row[index]);
		}
		return column;
	}

	vector_container_type& operator [](const size_type index) {
		return data_[index];
	}

	const vector_container_type& operator [](const size_type index) const {
		return data_[index];
	}

	constexpr size_type rows_cnt() const {
		return rows_cnt_;
	}

	constexpr size_type cols_cnt() const {
		return cols_cnt_;
	}

	Matrix multiply(const Matrix& rhs) const {
		Matrix result(rows_cnt_, rhs.cols_cnt_);
		for (size_type i = 0; i < rows_cnt_; i++) {
			for (size_type k = 0; k < rhs.cols_cnt_; k++) {
				for (size_type j = 0; j < rhs.rows_cnt_; j++) {
					result[i][k] += data_[i][j] * rhs[j][k];
				}
			}
		}
		return result;
	}

	Matrix operator *(const Matrix& rhs) const {
		return multiply(rhs);
	}

	Matrix& operator *=(const Matrix& rhs) {
		Matrix result = multiply(rhs);
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

	static Matrix identity_matrix(const size_type rows_cnt, const size_type cols_cnt) {
		Matrix result(rows_cnt, cols_cnt);
		for (size_type i = 0; i < std::min(rows_cnt, cols_cnt); ++i) {
			result[i][i] = 1;
		}
		return result;
	}

private:
	size_type rows_cnt_;
	size_type cols_cnt_;
	matrix_container_type data_;

	value_type mod_;
};

template <typename T>
typename Matrix<T>::size_type Matrix<T>::get_matrix_rank() const {
	Matrix<long double> tmp(*this);
	std::vector<bool> used(rows_cnt_, false);
	size_type rank = cols_cnt_;
	for (size_type col = 0; col < cols_cnt_; ++col) {
		size_type best_row = std::numeric_limits<size_type>::max();
		for (size_type row = 0; row < rows_cnt_; ++row) {
			if (!used[row] && !is_equal_to_zero(tmp[row][col])) {
				best_row = row;
				break;
			}
		}
		if (best_row == std::numeric_limits<size_type>::max()) {
			--rank;
			continue;
		}
		used[best_row] = true;
		for (size_type j = col + 1; j < cols_cnt_; ++j) {
			tmp[best_row][j] /= tmp[best_row][col];
		}
		for (size_type row = 0; row < rows_cnt_; ++row) {
			if (used[row] || is_equal_to_zero(tmp[row][col])) {
				continue;
			}
			for (size_type j = col + 1; j < cols_cnt_; ++j) {
				tmp[row][j] -= tmp[best_row][j] * tmp[row][col];
			}
		}
	}
	return rank;
}

template <typename T>
typename Matrix<T>::size_type Matrix<T>::get_matrix_rank(maximal_element_search_tag) const {
	Matrix<long double> tmp(*this);
	std::vector<bool> used(rows_cnt_, false);
	size_type rank = 0;
	for (size_type col = 0; col < cols_cnt_; ++col) {
		value_type max_value = EPS;
		size_type best_row = std::numeric_limits<size_type>::max();
		for (size_type row = 0; row < rows_cnt_; ++row) {
			if (!used[row] && umax(max_value, abs(tmp[row][col]))) {
				best_row = row;
			}
		}
		if (best_row == std::numeric_limits<size_type>::max()) {
			continue;
		}
		++rank;
		used[best_row] = true;
		for (size_type j = col + 1; j < cols_cnt_; ++j) {
			tmp[best_row][j] /= tmp[best_row][col];
		}
		for (size_type row = 0; row < rows_cnt_; ++row) {
			if (used[row] || is_equal_to_zero(tmp[row][col])) {
				continue;
			}
			for (size_type j = col + 1; j < cols_cnt_; ++j) {
				tmp[row][j] -= tmp[best_row][j] * tmp[row][col];
			}
		}
	}
	return rank;
}

template <typename T>
typename Matrix<T>::size_type Matrix<T>::get_matrix_rank(prime_modulo_calculations_tag, const value_type& mod) const {
	static_assert(std::is_integral<value_type>::value, "Integral type required to process calculations by prime modulo");
	Matrix<long long> tmp(*this);
	for (auto& row : tmp) {
		for (auto& col : row) {
			if (col < 0) {
				col += mod;
			}
		}
	}
	std::vector<bool> used(rows_cnt_, false);
	size_type rank = cols_cnt_;
	for (size_type col = 0; col < cols_cnt_; ++col) {
		size_type best_row = std::numeric_limits<size_type>::max();
		for (size_type row = 0; row < rows_cnt_; ++row) {
			if (!used[row] && !is_equal_to_zero(tmp[row][col])) {
				best_row = row;
				break;
			}
		}
		if (best_row == std::numeric_limits<size_type>::max()) {
			--rank;
			continue;
		}
		used[best_row] = true; 
		for (size_type j = col + 1; j < cols_cnt_; ++j) {
			mul_mod(tmp[best_row][j], inverseElement(tmp[best_row][col], mod), mod);
		}
		for (size_type row = 0; row < rows_cnt_; ++row) {
			if (used[row] || is_equal_to_zero(tmp[row][col])) {
				continue;
			}
			for (size_type j = col + 1; j < cols_cnt_; ++j) {
				long long x = tmp[best_row][j];
				mul_mod(x, tmp[row][col], mod);
				sub_mod(tmp[row][j], x, mod);
			}
		}
	}
	return rank;
}
