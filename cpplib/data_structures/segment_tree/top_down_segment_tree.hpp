#pragma once
#include <vector>

#include "base_segment_tree.hpp"

template<typename T, typename Merge, typename Update, typename ApplyUpdate, typename MergeUpdates>
class TopDownSegmentTree : public BaseSegmentTree<T, Merge> {
public:
	using apply_update_type = ApplyUpdate;
	using merge_type = Merge;
	using merge_updates_type = MergeUpdates;
	using update_type = Update;
	using value_type = T;

	using Base = BaseSegmentTree<value_type, merge_type>;
	using size_type = typename Base::size_type;

	explicit TopDownSegmentTree(
			const size_type elements_count,
			const value_type& default_value = value_type(0),
			const merge_type& merge = merge_type(),
			const update_type& default_update = update_type(),
			const apply_update_type& apply_update = apply_update_type(),
			const merge_updates_type& merge_updates = merge_updates_type()
	) :
			Base(elements_count, default_value, merge),
			updates_(size_, default_update),
			default_update_(default_update),
			apply_update_(apply_update),
			merge_updates_(merge_updates)
	{}

	void point_update(const size_type index, const update_type& update) {
		range_update(index, index, update);
	}

	void range_update(const size_type left, const size_type right, const update_type& update) {
		update_impl(1, 0, offset_ - 1, left, right, update);
	}

	value_type get(const size_type index) {
		return get(index, index);
	}

	value_type get(const size_type left, const size_type right) {
		return get_impl(1, 0, offset_ - 1, left, right);
	}

	std::vector<update_type>& updates() {
		return updates_;
	}

	const std::vector<update_type>& updates() const {
		return updates_;
	}

	constexpr update_type default_update() const {
		return default_update_;
	}

protected:
	using Base::offset_;
	using Base::size_;
	using Base::data_;
	using Base::merge_;
	using Base::default_value_;

private:
	std::vector<update_type> updates_;
	const update_type default_update_;
	const apply_update_type apply_update_;
	const merge_updates_type merge_updates_;

	static constexpr size_type get_mid(const size_type cur_left, const size_type cur_right) {
		return (cur_left + cur_right) / 2;
	}

	value_type get_impl(const size_type v, const size_type cur_left, const size_type cur_right, const size_type left, const size_type right) {
		if (right < cur_left || left > cur_right) {
			return default_value_;
		}
		if (left <= cur_left && cur_right <= right) {
			return data_[v];
		}
		push_impl(v, cur_left, cur_right);
		const size_type mid = get_mid(cur_left, cur_right);
		return merge_(
				get_impl(v << 1, cur_left, mid, left, right),
				get_impl((v << 1) ^ 1, mid + 1, cur_right, left, right)
		);
	}

	void update_impl(const size_type v, const size_type cur_left, const size_type cur_right, const size_type left, const size_type right, const update_type& update) {
		if (right < cur_left || left > cur_right) {
			return;
		}
		if (left <= cur_left && cur_right <= right) {
			data_[v] = apply_update_(data_[v], update, cur_left, cur_right);
			updates_[v] = merge_updates_(updates_[v], update);
			return;
		}
		push_impl(v, cur_left, cur_right);

		const size_type mid = get_mid(cur_left, cur_right);
		update_impl(v << 1, cur_left, mid, left, right, update);
		update_impl((v << 1) ^ 1, mid + 1, cur_right, left, right, update);

		this->merge_children(v);
	}

	void push_impl(const size_type v, const size_type cur_left, const size_type cur_right) {
		const size_type mid = get_mid(cur_left, cur_right);

		data_[v << 1] = apply_update_(data_[v << 1], updates_[v], cur_left, mid);
		updates_[v << 1] = merge_updates_(updates_[v << 1], updates_[v]);

		data_[(v << 1) ^ 1] = apply_update_(data_[(v << 1) ^ 1], updates_[v], mid + 1, cur_right);
		updates_[(v << 1) ^ 1] = merge_updates_(updates_[(v << 1) ^ 1], updates_[v]);

		updates_[v] = default_update_;
	}

};

template<typename T, typename Update = T>
struct Applier {
	using size_type = std::size_t;
	using update_type = Update;
	using value_type = T;

	virtual value_type operator()(const value_type& value, const update_type& update, const size_type left, const size_type right) const = 0;
};

template<typename Update>
struct Merger {
	using update_type = Update;

	virtual update_type operator()(const update_type& acc_updates, const update_type& update) const = 0;
};
