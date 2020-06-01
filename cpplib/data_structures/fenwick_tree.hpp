#pragma once
#include <algorithm>
#include <cstdlib>
#include <vector>

template<typename T>
class FenwickTreeSum {
public:
	using value_type = T;
	using size_type = std::size_t;
	using container_type = std::vector<value_type>;

	explicit FenwickTreeSum(const size_type n) : size_(n), data_(n, 0) {}

	explicit FenwickTreeSum(const std::vector<value_type>& a) : size_(a.size()) {
		for (size_type i = 0; i < size_; ++i) {
			inc(i, a[i]);
		}
	}

	void clear() {
		data_.assign(size_, 0);
	}

	size_type size() const {
		return size_;
	}

	container_type& data() {
		return data_;
	}

	const container_type& data() const {
		return data_;
	}

	void inc(int v, const value_type delta) {
		for (; v < size_; v = (v | (v + 1))) {
			data_[v] += delta;
		}
	}

	value_type query(int v) const
	// returns sum for range [0, index]
	{
		value_type res = 0;
		for (; v >= 0; v = (v & (v + 1)) - 1) {
			res += data_[v];
		}
		return res;
	}

	value_type query(const int left, const int right) const
	// returns sum for range [left, right]
	{
		if (right < left) {
			return 0;
		}
		return query(right) - query(left - 1);
	}

private:
	size_type size_;
	container_type data_;
};

template<class T> class FenwickTreeSumRangeUpdates {
public:
	T * add, * mul;
	int N;

	FenwickTreeSumRangeUpdates(const int N) : N(N) {
		add = new T[N];
		mul = new T[N];
		std::fill_n(add, N, 0);
		std::fill_n(mul, N, 0);
	}

	~FenwickTreeSumRangeUpdates() {
		delete[] add;
		delete[] mul;
	}

	void clear() {
		std::fill_n(add, N, 0);
		std::fill_n(mul, N, 0);
	}

	void setSize(const int n) {
		N = n;
	}

	void build(T a[], const int n) {
		N = n;
		for (int i = 0; i < N; ++i) {
			update(i, i, a[i]);
		}
	}

	void update(const int left, const int right, const T by) {
		if (left > right) {
			return;
		}
		_update(left, by, -by * (left - 1));
		_update(right, -by, by * right);
	}

	T query(int at) const {
		T resMul = 0;
		T resAdd = 0;
		int start = at;
		while (at >= 0) {
			resMul += mul[at];
			resAdd += add[at];
			at = (at & (at + 1)) - 1;
		}
		return resMul * start + resAdd;
	}

	T query(const int left, const int right) const {
		return query(right) - query(left - 1);
	}

private:
	void _update(int at, const T updMul, const T updAdd) {
		while (at < N) {
			mul[at] += updMul;
			add[at] += updAdd;
			at |= (at + 1);
		}
	}

};