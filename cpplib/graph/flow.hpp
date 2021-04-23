#pragma once
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

    class Edge {
    public:
        Edge(const vertex_id_type from, const vertex_id_type to, const weight_type capacity, const weight_type flow) :
            from(from), to(to), cap(capacity), flow(flow)
        {}

        vertex_id_type from;
        vertex_id_type to;
        weight_type cap;
        weight_type flow;
    };

    explicit DinicFlow(const size_type n) : graph(n), queue(n), pointer(n), dist(n), used(n) {}

    void add_directed_edge(const vertex_id_type from, const vertex_id_type to, const weight_type capacity) {
        return add_bidirectional_edge(from, to, capacity, 0);
    }

    void add_bidirectional_edge(const vertex_id_type from, const vertex_id_type to, const weight_type capacity, const weight_type backward_capacity) {
        push_edge(from, to, capacity, 0);
        push_edge(to, from, backward_capacity, 0);
    }

    [[nodiscard]] Edge get_edge(const edge_id_type id) const {
        return edges[id];
    }

    [[nodiscard]] static weight_type weight_infinity() {
        return std::numeric_limits<weight_type>::max() / 2;
    }

    weight_type findFlow(const vertex_id_type from, const vertex_id_type to, const weight_type infinity = weight_infinity()) {
        weight_type flow = 0;
        while (bfs(from, to)) {
            pointer.assign(pointer.size(), 0);
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
        return edges.size();
    }

private:
    void push_edge(const vertex_id_type from, const vertex_id_type to, const weight_type capacity, const weight_type backward_capacity) {
        graph[from].emplace_back(edges.size());
        edges.emplace_back(from, to, capacity, 0);
    }

    bool bfs(const vertex_id_type from, const vertex_id_type to) {
        used.assign(used.size(), false);
        queue.clear();
        queue.push(from);
        used[from] = true;
        while (!queue.empty()) {
            const vertex_id_type vertex = queue.pop_front();
            for (const edge_id_type id : graph[vertex]) {
                const Edge& edge = edges[id];
                if (used[edge.to] || edge.cap == edge.flow) {
                    continue;
                }
                used[edge.to] = true;
                dist[edge.to] = dist[vertex] + 1;
                queue.push(edge.to);
            }
        }
        return used[to];
    }

    weight_type dfs(const vertex_id_type vertex, const vertex_id_type to, const weight_type mx) {
        if (mx == 0 || vertex == to) {
            return mx;
        }
        for (edge_id_type& i = pointer[vertex]; i < graph[vertex].size(); ++i) {
            const edge_id_type id = graph[vertex][i];
            Edge& e = edges[id];
            if (dist[e.to] == dist[vertex] + 1) {
                const weight_type pushed = dfs(e.to, to, std::min(mx, e.cap - e.flow));
                if (pushed != 0) {
                    e.flow += pushed;
                    edges[id ^ 1].flow -= pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    std::vector<std::vector<edge_id_type>> graph;
    std::vector<Edge> edges;

    Queue<vertex_id_type> queue;
    std::vector<edge_id_type> pointer;
    std::vector<weight_type> dist;
    std::vector<bool> used;
};
