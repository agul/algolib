#pragma once
#include <algorithm>
#include <numeric>
#include <vector>

#include "maths/random.hpp"

class DSU {
public:
    using size_type = std::size_t;
    using vertex_id_type = std::size_t;
    using container_type = std::vector<vertex_id_type>;

    DSU() : DSU(0) {}

    explicit DSU(const size_type vertices_count) {
        init(vertices_count);
    }

    void init(const size_type vertices_count) {
        parent_.resize(vertices_count);
        std::iota(parent_.begin(), parent_.end(), 0);
        sets_count_ = vertices_count;
    }

    vertex_id_type find_set(const vertex_id_type vertex) {
        if (vertex != parent_[vertex]) {
            parent_[vertex] = find_set(parent_[vertex]);
        }
        return parent_[vertex];
    }

    bool unite(const vertex_id_type a, const vertex_id_type b) {
        vertex_id_type x = find_set(a);
        vertex_id_type y = find_set(b);
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

    [[nodiscard]] size_type size() const {
        return parent_.size();
    }

    [[nodiscard]] size_type sets_count() const {
        return sets_count_;
    }

    [[nodiscard]] const container_type& data() const {
        return parent_;
    }

private:
    container_type parent_;
    size_type sets_count_;
};