#pragma once
#include "Head.h"

template<class T> class SparseTableCmp {
public:
	T ** data;
	int * logTable;

	SparseTableCmp(const int N, function<bool(const T&, const T&)> less) : N(N), less(less) {
		logTable = new int[N + 1];
		logTable[0] = logTable[1] = 0;
		for (int i = 2; i <= N; ++i) {
			logTable[i] = logTable[i >> 1] + 1;
		}
		data = new T*[logTable[N] + 1];
		for (int i = 0; i <= logTable[N]; ++i) {
			data[i] = new T[N];
		}
	}

	~SparseTableCmp() {
		for (int i = 0; i <= logTable[N]; ++i) {
			delete[] data[i];
		}
		delete[] data;
		delete[] logTable;
	}

	void setComparator(function<bool(const T&, const T&)> cmp) {
		less = cmp;
	}

	void build(T a[]) {
		arr = a;
		for (int i = 0; i < N; ++i) {
			data[0][i] = i;
		}
		for (int k = 1, tt = 2; tt < N; ++k, tt <<= 1) {
			for (int i = 0; i + tt <= N; ++i) {
				int x = data[k - 1][i];
				int y = data[k - 1][i + (tt >> 1)];
				data[k][i] = (less(arr[x], arr[y]) ? x : y);
			}
		}
	}

	int query(int l, int r) {
		int k = logTable[r - l];
		int x = data[k][l];
		int y = data[k][r - (1 << k) + 1];
		return (less(arr[x], arr[y]) ? x : y);
	}

private:
	SparseTableCmp();

	function<bool(const T&, const T&)> less;
	T * arr;
	int N;

};