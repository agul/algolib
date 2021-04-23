#pragma once
#include <cstddef>
#include <vector>

#include "undirected_graph.hpp"

class LCA {
public:
    using mask_type = uint32_t;
    using vertex_id_type = std::size_t;
    using size_type = std::size_t;
    using timer_type = std::size_t;

    template<typename T, mask_type MASK>
    explicit LCA(const UndirectedGraph<T, MASK>& graph, const vertex_id_type starting_vertex = 0) :
            up_(graph.vertices_count()),
            tin_(graph.vertices_count()),
            tout_(graph.vertices_count()),
            timer_(0)
    {
        log_ = 1;
        while ((1 << log_) <= graph.vertices_count()) {
            ++log_;
        }
        for (auto& it : up_) {
            it.resize(log_ + 1);
        }
        dfs(graph, starting_vertex, starting_vertex);
    }

    [[nodiscard]] const std::vector<timer_type>& tin() const {
        return tin_;
    }

    [[nodiscard]] const std::vector<timer_type>& tout() const {
        return tout_;
    }

    [[nodiscard]] const std::vector<std::vector<vertex_id_type>>& up() const {
        return up_;
    }

    [[nodiscard]] timer_type tin(const vertex_id_type vertex) const {
        return tin_[vertex];
    }

    [[nodiscard]] timer_type tout(const vertex_id_type vertex) const {
        return tout_[vertex];
    }

    [[nodiscard]] bool upper(const vertex_id_type lhs, const vertex_id_type rhs) const {
        const timer_type time_in = tin_[rhs];
        return tin_[lhs] <= time_in && tout_[lhs] >= time_in;
    }

    [[nodiscard]] vertex_id_type query(const vertex_id_type lhs, const vertex_id_type rhs) const {
        if (upper(lhs, rhs)) {
            return lhs;
        }
        if (upper(rhs, lhs)) {
            return rhs;
        }
        vertex_id_type vertex = lhs;
        for (const size_type i : inclusiveDownrange(log_)) {
            if (!upper(up_[vertex][i], rhs)) {
                vertex = up_[vertex][i];
            }
        }
        return up_[vertex][0];
    }

private:
    template<typename T, mask_type MASK>
    void dfs(const UndirectedGraph<T, MASK>& graph, const vertex_id_type vertex, const vertex_id_type parent) {
        tin_[vertex] = timer_++;
        up_[vertex][0] = parent;
        for (size_type i = 1; i <= log_; ++i) {
            up_[vertex][i] = up_[up_[vertex][i - 1]][i - 1];
        }
        for (const auto& it : graph.edges(vertex)) {
            const vertex_id_type to = it.to();
            if (to == parent) {
                continue;
            }
            dfs(graph, to, vertex);
        }
        tout_[vertex] = timer_++;
    }

    std::vector<std::vector<vertex_id_type>> up_;
    std::vector<timer_type> tin_;
    std::vector<timer_type> tout_;
    size_type log_;
    timer_type timer_;
};
