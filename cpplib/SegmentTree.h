#pragma once
#include "Head.h"

template<typename T> 
class SegmentTree {
public:

	/* enum Type {
		NONE,
		INCREMENT, // Increment updates on a segment.

	}; */

	std::vector<T> data;

	SegmentTree(const size_t N, const std::function<const T(const T&, const T&)>& pred, const T& neutral_ = static_cast<T>(0)) : N_(N), pred_(pred), neutral_(neutral_) {
		offset_ = 1 << (31 - clz(N) + ((N & (N - 1)) == 0 ? 0 : 1));
		size_ = offset_ << 1;
		data.resize(size_);
		init();
	}

	SegmentTree() = delete;

	SegmentTree(SegmentTree&) = default;
	SegmentTree(SegmentTree&&) = default;

	SegmentTree& operator =(SegmentTree&) = default;
	SegmentTree& operator =(SegmentTree&&) = default;

	~SegmentTree() = default;

	size_t offset() const {
		return offset_;
	}

	size_t size() const {
		return size_;
	}

	size_t elements_count() const {
		return N_;
	}

	void init() {
		std::fill_n(data.begin(), data.size(), neutral_);
	}

	void build(T a[]) {
		std::copy(a, a + N_, data.begin() + offset_);
		for (size_t i = offset_ - 1; i >= 1; --i) {
			data[i] = pred_(data[i << 1], data[(i << 1) ^ 1]);
		}
	}

	T query(const size_t left, const size_t right) const {
		T res = neutral_;
		for (size_t l = left + offset_, r = right + offset_; l <= r; l >>= 1, r >>= 1) {
			if (l & 1) {
				res = pred_(data[l++], res);
			}
			if ((r & 1) == 0) {
				res = pred_(data[r--], res);
			}
		}
		return res;
	}

	void update(const size_t position, const T& value) {
		size_t pos = position + offset_;
		data[pos] = value;
		for (pos >>= 1; pos >= 1; pos >>= 1) {
			data[pos] = pred_(data[pos << 1], data[(pos << 1) ^ 1]);
		}
	}

private:
	const std::function<const T(const T&, const T&)> pred_;
	const T neutral_;
	size_t offset_;
	size_t size_;
	size_t N_;

};
