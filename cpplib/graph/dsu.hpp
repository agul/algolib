#pragma once
#include <algorithm>
#include <numeric>

#include "maths/random.hpp"

class DSU {
public:
    using size_type = std::size_t;

    DSU() : DSU(0) {}

    explicit DSU(const size_type vertices_count) {
        init(vertices_count);
    }

    void init(const size_type vertices_count) {
        parent_.resize(vertices_count);
        std::iota(parent_.begin(), parent_.end(), 0);
        sets_count_ = vertices_count;
    }

    size_type find_set(const size_type vertex) {
        if (vertex != parent_[vertex]) {
            parent_[vertex] = find_set(parent_[vertex]);
        }
        return parent_[vertex];
    }

    bool unite(const size_type a, const size_type b) {
        size_type x = find_set(a);
        size_type y = find_set(b);
        if (x == y) {
            return false;
        }
        if (Random::get(2) == 0) {
            std::swap(x, y);
        }
        parent_[x] = y;
        --sets_count_;
        return true;
    }

    DSU& finalize() {
        for (auto& it : parent_) {
            it = find_set(it);
        }
        return *this;
    }

    size_type size() const {
        return parent_.size();
    }

    size_type sets_count() const {
        return sets_count_;
    }

    const std::vector<size_type>& data() const {
        return parent_;
    }

private:
    std::vector<size_type> parent_;
    size_type sets_count_;
};