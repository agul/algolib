#pragma once
#include "Head.h"

template<class T> class FenwickTreeSum {
public:
	T *data;
	int N;

	FenwickTreeSum(const int N) : N(N) {
		data = new T[N];
		fill_n(data, N, 0);
	}

	~FenwickTreeSum() {
		delete[] data;
	}

	void clear() {
		fill_n(data, N, 0);
	}

	void build(T a[]) {
		for (int i = 0; i < N; ++i) {
			inc(i, a[i]);
		}
	}

	void inc(int v, const T delta) {
		for (; v < N; v = (v | (v + 1))) {
			data[v] += delta;
		}
	}

	T query(int v) {
		T res = 0;
		for (; v >= 0; v = (v & (v + 1)) - 1) {
			res += data[v];
		}
		return res;
	}

	T query(const int l, const int r) {
		if (r < l) {
			return 0;
		}
		return query(r) - query(l - 1);
	}

};