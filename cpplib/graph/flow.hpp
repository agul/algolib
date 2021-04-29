#pragma once
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <limits>
#include <vector>

#include "collections/queue/queue.hpp"

template<typename T>
class DinicFlow {
public:
    using weight_type = T;
    using edge_id_type = std::size_t;
    using vertex_id_type = std::size_t;
    using size_type = std::size_t;

    struct Edge {
        vertex_id_type from;
        vertex_id_type to;
        weight_type cap;
        weight_type flow;
    };

    explicit DinicFlow(const size_type vertices_count) :
            graph_(vertices_count),
            queue_(vertices_count),
            pointer_(vertices_count),
            dist_(vertices_count),
            used_(vertices_count)
    {}

    void add_directed_edge(const vertex_id_type from, const vertex_id_type to, const weight_type capacity) {
        return add_bidirectional_edge(from, to, capacity, 0);
    }

    void add_bidirectional_edge(const vertex_id_type from, const vertex_id_type to, const weight_type capacity, const weight_type backward_capacity) {
        push_edge(from, to, capacity, 0);
        push_edge(to, from, backward_capacity, 0);
    }

    [[nodiscard]] Edge get_edge(const edge_id_type id) const {
        return edges_[id];
    }

    [[nodiscard]] static weight_type weight_infinity() {
        return std::numeric_limits<weight_type>::max() / 2;
    }

    weight_type find_flow(const vertex_id_type from, const vertex_id_type to, const weight_type infinity = weight_infinity()) {
        weight_type flow = 0;
        while (bfs(from, to)) {
            pointer_.assign(pointer_.size(), 0);
            while (true) {
                const weight_type pushed = dfs(from, to, infinity);
                if (pushed == 0) {
                    break;
                }
                flow += pushed;
            }
        }
        return flow;
    }

    [[nodiscard]] size_type edges_count() const {
        return edges_.size();
    }

private:
    void push_edge(const vertex_id_type from, const vertex_id_type to, const weight_type capacity, const weight_type backward_capacity) {
        graph_[from].emplace_back(edges_.size());
        edges_.emplace_back(Edge{from, to, capacity, backward_capacity});
    }

    bool bfs(const vertex_id_type from, const vertex_id_type to) {
        used_.assign(used_.size(), false);
        queue_.clear();
        queue_.push(from);
        used_[from] = true;
        while (!queue_.empty()) {
            const vertex_id_type vertex = queue_.pop_front();
            for (const edge_id_type id : graph_[vertex]) {
                const Edge& edge = edges_[id];
                if (used_[edge.to] || edge.cap == edge.flow) {
                    continue;
                }
                used_[edge.to] = true;
                dist_[edge.to] = dist_[vertex] + 1;
                queue_.push(edge.to);
            }
        }
        return used_[to];
    }

    weight_type dfs(const vertex_id_type vertex, const vertex_id_type to, const weight_type max_capacity) {
        if (max_capacity == 0 || vertex == to) {
            return max_capacity;
        }
        for (edge_id_type& i = pointer_[vertex]; i < graph_[vertex].size(); ++i) {
            const edge_id_type id = graph_[vertex][i];
            Edge& e = edges_[id];
            if (dist_[e.to] == dist_[vertex] + 1) {
                const weight_type pushed = dfs(e.to, to, std::min(max_capacity, e.cap - e.flow));
                if (pushed != 0) {
                    e.flow += pushed;
                    edges_[id ^ 1].flow -= pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    std::vector<std::vector<edge_id_type>> graph_;
    std::vector<Edge> edges_;

    Queue<vertex_id_type> queue_;
    std::vector<edge_id_type> pointer_;
    std::vector<weight_type> dist_;
    std::vector<bool> used_;
};
