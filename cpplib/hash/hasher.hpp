#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <vector>

#include "maths/maths.hpp"
#include "maths/random.hpp"
#include "string/string_view.hpp"
#include "double_hasher.hpp"

template<typename T, typename Container = StringView>
class Hasher {
public:
	using size_type = std::size_t;
	using container_type = Container;
	using value_type = T;

	explicit Hasher(const container_type& data) : size_(data.size()), hash_(size_ + 1), data_(data) {
		const value_type kHashPoint = Random::get(307, 877117);
		deg_ = calc_powers(kHashPoint, size_ + 1);

		hash_[0] = 0;
		for (size_type i = 0; i < data_.size(); ++i) {
			hash_[i + 1] = hash_[i] * kHashPoint + data_[i];
		}
	}

	value_type get_hash(const size_type left, const size_type right) const
	// returns hash for range [left, right]
	{
		return hash_[right + 1] - hash_[left] * deg_[right - left + 1];
	}

	size_type lcp(const size_type a, const size_type b) const {
		size_type L = 0;
		size_type R = size_ - std::max(a, b) + 1;
		while (R - L > 1) {
			const size_type md = (L + R + 1) / 2;
			if (get_hash(a, a + md - 1) == get_hash(b, b + md - 1)) {
				L = md;
			} else {
				R = md;
			}
		}
		return L;
	}

	bool compare_substrings(const size_type a, const size_type b) const {
		return compare_substrings_impl(a, b);
	}

private:
	size_type size_;
	std::vector<value_type> deg_;
	std::vector<value_type> hash_;
	const container_type& data_;

	bool compare_substrings_impl(size_type a, size_type b) const {
		const size_type max_substring_length = size_ - std::max(a, b);
		const size_type lcp_length = lcp(a, b);
		return lcp_length < max_substring_length ? data_[a + lcp_length] < data_[b + lcp_length] : b < a;
	}

};
