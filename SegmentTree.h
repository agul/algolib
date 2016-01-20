#pragma once
#include "Head.h"

template<class T> class SegmentTreeCmp {
public:

	T * data;
	int offset, size, N;

	SegmentTreeCmp(const int N, function<bool(const T&, const T&)> less, const T& neutral) : N(N), less(less), neutral(neutral) {
		if (N & (N - 1)) {
			offset = 1 << (33 - clz(N)); // 32 - clz(a) + 1;
		}
		else {
			offset = 1 << (32 - clz(N));
		}
		size = offset << 1;
		data = new T[size];
	}

	~SegmentTreeCmp() {
		delete[] data;
	}

	void setComparator(function<bool(const T&, const T&)> cmp) {
		less = cmp;
	}

	void setNeutral(const T& val) {
		neutral = val;
	}

	void build(T a[]) {
		arr = a;
		fill_n(data, size, neutral);
		for (int i = 0; i < N; ++i) {
			data[offset + i] = a[i];
		}
		for (int i = offset - 1; i >= 1; --i) {
			const T& x = data[i << 1], &y = data[(i << 1) ^ 1];
			data[i] = (less(x, y) ? x : y);
		}
	}

	T query(int l, int r) {
		l += offset;
		r += offset;
		T res = neutral;
		while (l <= r) {
			if (l & 1) {
				const T& cur = data[l++];
				if (less(cur, res)) {
					res = cur;
				}
			}
			if (!(r & 1)) {
				const T& cur = data[r--];
				if (less(cur, res)) {
					res = cur;
				}
			}
			l >>= 1;
			r >>= 1;
		}
		return res;
	}

	void update(int pos, const T& val) {
		pos += offset;
		data[pos] = val;
		pos >>= 1;
		while (pos >= 1) {
			const T &x = data[pos << 1], &y = data[(pos << 1) ^ 1];
			data[pos] = (less(x, y) ? x : y);
			pos >>= 1;
		}
	}

private:
	SegmentTreeCmp();

	function<bool(const T&, const T&)> less;
	T * arr, neutral;

};
