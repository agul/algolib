#pragma once
#include <algorithm>
#include <numeric>

#include "maths/maths.hpp"

class DSU {
public:
	DSU() = default;

	void init(const size_t vertices_count) {
		srand(static_cast<uint32_t>(Random::GetRandSeed()));
		parent_.resize(vertices_count);
		std::iota(parent_.begin(), parent_.end(), 0);
		sets_count_ = vertices_count;
	}

	size_t find_set(const size_t vertex) {
		if (vertex != parent_[vertex]) {
			parent_[vertex] = find_set(parent_[vertex]);
		}
		return parent_[vertex];
	}

	bool unite(const size_t a, const size_t b) {
		size_t x = find_set(a);
		size_t y = find_set(b);
		if (x == y) {
			return false;
		}
		if ((rand() & 1) != 0) {
			std::swap(x, y);
		}
		parent_[x] = y;
		--sets_count_;
		return true;
	}

	void finalize() {
		for (auto& it : parent_) {
			it = find_set(it);
		}
	}

	size_t size() const {
		return parent_.size();
	}

	size_t sets_count() const {
		return sets_count_;
	}

	const std::vector<size_t>& data() const {
		return parent_;
	}

private:
	std::vector<size_t> parent_;
	size_t sets_count_;

};