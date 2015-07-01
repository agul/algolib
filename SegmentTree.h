#pragma once
#include "Head.h"

template<class T> struct SegmentTreeMin {

	int offset, size, *data, N;

	SegmentTreeMin(const int N) : N(N) {
		for (offset = 1; offset < N; offset <<= 1);
		size = offset << 1;
		data = new int[size];
	}

	~SegmentTreeMin() {
		delete[] data;
	}

	void build(T a[]) {
		memset(data, -1, size * sizeof(int));
		for (int i = 0; i < N; ++i) {
			data[offset + i] = i;
		}
		for (int i = offset - 1; i >= 1; --i) {
			data[i] = cmp(a, data[i << 1], data[(i << 1) ^ 1]);
		}
	}

	int query(T a[], int l, int r) {
		l += offset;
		r += offset;
		int res = -1;
		while (l <= r) {
			if (l & 1) {
				res = cmp(a, res, data[l++]);
			}
			if (!(r & 1)) {
				res = cmp(a, res, data[r--]);
			}
			l >>= 1;
			r >>= 1;
		}
		return res;
	}

	int cmp(T a[], const int x, const int y) {
		if (x == -1) {
			return y;
		}
		if (y == -1) {
			return x;
		}
		return (a[x] < a[y] ? x : y);
	}

};