#pragma once
#include <vector>

#include "base_segment_tree.hpp"

template<typename T, typename Merge, typename Update, typename ApplyUpdate, typename MergeUpdates>
class TopDownSegmentTree : public BaseSegmentTree<T, Merge> {
public:
	using Base = BaseSegmentTree<T, Merge>;

	explicit TopDownSegmentTree(
			const size_t N,
			const T& default_value = T(0),
			const Merge& merge = Merge(),
			const Update& default_update = Update(),
			const ApplyUpdate& apply_update = ApplyUpdate(),
			const MergeUpdates& merge_updates = MergeUpdates()
	) :
			Base(N, default_value, merge),
			updates_(size_, default_update),
			default_update_(default_update),
			apply_update_(apply_update),
			merge_updates_(merge_updates)
	{
		init();
	}

	void init() {
		std::fill_n(updates_.begin(), updates_.size(), 0);
	}

	void point_update(const size_t index, const Update& update) {
		range_update(index, index, update);
	}

	void range_update(const size_t left, const size_t right, const Update& update) {
		update_impl(1, 0, offset_ - 1, left, right, update);
	}

	T get(const size_t index) {
		return get(index, index);
	}

	T get(const size_t left, const size_t right) {
		return get_impl(1, 0, offset_ - 1, left, right);
	}

protected:
	using Base::offset_;
	using Base::size_;
	using Base::data_;
	using Base::merge_;
	using Base::default_value_;

private:
	std::vector<Update> updates_;
	const Update default_update_;
	const ApplyUpdate apply_update_;
	const MergeUpdates merge_updates_;

	static constexpr size_t get_mid(const size_t cur_left, const size_t cur_right) {
		return (cur_left + cur_right) / 2;
	}

	T get_impl(const size_t v, const size_t cur_left, const size_t cur_right, const size_t left, const size_t right) {
		if (right < cur_left || left > cur_right) {
			return default_value_;
		}
		if (left <= cur_left && cur_right <= right) {
			return data_[v];
		}
		push_impl(v, cur_left, cur_right);
		const size_t mid = get_mid(cur_left, cur_right);
		return merge_(
				get_impl(v << 1, cur_left, mid, left, right),
				get_impl((v << 1) ^ 1, mid + 1, cur_right, left, right)
		);
	}

	void update_impl(const size_t v, const size_t cur_left, const size_t cur_right, const size_t left, const size_t right, const Update& update) {
		if (right < cur_left || left > cur_right) {
			return;
		}
		if (left <= cur_left && cur_right <= right) {
			data_[v] = apply_update_(data_[v], update, cur_left, cur_right);
			updates_[v] = merge_updates_(updates_[v], update);
			return;
		}
		push_impl(v, cur_left, cur_right);

		const size_t mid = get_mid(cur_left, cur_right);
		update_impl(v << 1, cur_left, mid, left, right, update);
		update_impl((v << 1) ^ 1, mid + 1, cur_right, left, right, update);

		this->merge_children(v);
	}

	void push_impl(const size_t v, const size_t cur_left, const size_t cur_right) {
		const size_t mid = get_mid(cur_left, cur_right);

		data_[v << 1] = apply_update_(data_[v << 1], updates_[v], cur_left, mid);
		updates_[v << 1] = merge_updates_(updates_[v << 1], updates_[v]);

		data_[(v << 1) ^ 1] = apply_update_(data_[(v << 1) ^ 1], updates_[v], mid + 1, cur_right);
		updates_[(v << 1) ^ 1] = merge_updates_(updates_[(v << 1) ^ 1], updates_[v]);

		updates_[v] = default_update_;
	}

};

template<typename T, typename Update = T>
struct Applier {
	virtual T operator()(const T& value, const Update& update, const size_t left, const size_t right) const = 0;
};

template<typename Update>
struct Merger {
	virtual Update operator()(const Update& acc_updates, const Update& update) const = 0;
};
