#pragma once
#include <vector>

template<typename T, typename Merge>
class BaseSegmentTree {
public:
	using size_type = std::size_t;
	using value_type = T;
	using merge_type = Merge;

	explicit BaseSegmentTree(
			const size_type elements_count,
			const value_type& default_value = value_type(0),
			const merge_type& merge = merge_type()
	) :
			elements_count_(elements_count),
			offset_(calculate_offset(elements_count_)),
			size_(offset_ << 1),
			data_(size_, default_value),
			merge_(merge),
			default_value_(default_value)
	{}

	static constexpr size_type calculate_offset(const size_type N) {
		return 1 << (31 - clz(N) + ((N & (N - 1)) == 0 ? 0 : 1));
	}

	constexpr size_type offset() const {
		return offset_;
	}

	constexpr size_type size() const {
		return size_;
	}

	constexpr size_type elements_count() const {
		return elements_count_;
	}

	constexpr value_type default_value() const {
		return default_value_;
	}

	std::vector<value_type>& data() {
		return data_;
	}

	const std::vector<value_type>& data() const {
		return data_;
	}

	virtual void build(const value_type* a) {
		std::copy(a, a + elements_count_, data_.begin() + offset_);
		for (size_type i = offset_ - 1; i >= 1; --i) {
			merge_children(i);
		}
	}

	virtual void build(const std::vector<value_type>& a) {
		build(a.data());
	}

protected:
	size_type elements_count_;
	size_type offset_;
	size_type size_;

	std::vector<value_type> data_;
	const merge_type merge_;
	const value_type default_value_;

	void merge_children(const size_type v) {
		data_[v] = merge_(data_[v << 1], data_[(v << 1) ^ 1]);
	}
};
