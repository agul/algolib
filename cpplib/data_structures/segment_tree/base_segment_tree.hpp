#pragma once
#include <vector>

template<typename T, typename Merge>
class BaseSegmentTree {
public:
	explicit BaseSegmentTree(
			const size_t N,
			const T& default_value = T(0),
			const Merge& merge = Merge()
	) :
			N_(N),
			offset_(calculate_offset(N)),
			size_(offset_ << 1),
			data_(size_, default_value),
			merge_(merge),
			default_value_(default_value)
	{
		init();
	}

	static constexpr size_t calculate_offset(const size_t N) {
		return 1 << (31 - clz(N) + ((N & (N - 1)) == 0 ? 0 : 1));
	}

	constexpr size_t offset() const {
		return offset_;
	}

	constexpr size_t size() const {
		return size_;
	}

	constexpr size_t elements_count() const {
		return N_;
	}

	constexpr T default_value() const {
		return default_value_;
	}

	std::vector<T>& data() const {
		return data_;
	}

	virtual void init() {
		std::fill_n(data_.begin(), data_.size(), default_value_);
	}

	virtual void build(const T* a) {
		std::copy(a, a + N_, data_.begin() + offset_);
		for (size_t i = offset_ - 1; i >= 1; --i) {
			merge_children(i);
		}
	}

	virtual void build(const std::vector<T>& a) {
		build(a.data());
	}

	void merge_children(const size_t v) {
		data_[v] = merge_(data_[v << 1], data_[(v << 1) ^ 1]);
	}

protected:
	size_t N_;
	size_t offset_;
	size_t size_;

	std::vector<T> data_;

	const Merge merge_;

	const T default_value_;
	
};
