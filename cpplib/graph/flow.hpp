#pragma once
#include <cstddef>
#include <cstring>
#include <limits>
#include <vector>

#include "collections/queue/queue.hpp"

template<typename T>
class DinicFlow {
public:
    class Edge {
    public:
        Edge(const size_t from, const size_t to, const T capacity, const T flow) : from(from), to(to), cap(capacity), flow(flow) {}

        size_t from;
        size_t to;
        T cap;
        T flow;
    };

    explicit DinicFlow(const size_t n) : graph(n), queue(n), pointer(n), dist(n), used(n) {}

    void add_directed_edge(const size_t from, const size_t to, const T capacity) {
        return add_bidirectional_edge(from, to, capacity, 0);
    }

    void add_bidirectional_edge(size_t from, size_t to, T capacity, T backward_capacity) {
        push_edge(from, to, capacity, 0);
        push_edge(to, from, backward_capacity, 0);
    }

    [[nodiscard]] Edge get_edge(const size_t id) const {
        return edges[id];
    }

    static T weight_infinity() {
        return std::numeric_limits<T>::max() / 2;
    }

    T findFlow(const size_t from, const size_t to, const T infinity = weight_infinity()) {
        T flow = 0;
        while (bfs(from, to)) {
            pointer.assign(pointer.size(), 0);
            while (true) {
                const T pushed = dfs(from, to, infinity);
                if (pushed == 0) {
                    break;
                }
                flow += pushed;
            }
        }
        return flow;
    }

    [[nodiscard]] size_t edges_count() const {
        return edges.size();
    }

private:
    void push_edge(const size_t from, const size_t to, const T capacity, const T backwardCapacity) {
        graph[from].emplace_back(edges.size());
        edges.emplace_back(from, to, capacity, 0);
    }

    bool bfs(const size_t from, const size_t to) {
        used.assign(used.size(), false);
        queue.clear();
        queue.push(from);
        used[from] = true;
        while (!queue.empty()) {
            const size_t vertex = queue.pop_front();
            for (const size_t id : graph[vertex]) {
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

    T dfs(const size_t vertex, const size_t to, const T mx) {
        if (mx == 0 || vertex == to) {
            return mx;
        }
        for (size_t& i = pointer[vertex]; i < graph[vertex].size(); ++i) {
            const size_t id = graph[vertex][i];
            Edge& e = edges[id];
            if (dist[e.to] == dist[vertex] + 1) {
                const T pushed = dfs(e.to, to, std::min(mx, e.cap - e.flow));
                if (pushed != 0) {
                    e.flow += pushed;
                    edges[id ^ 1].flow -= pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    std::vector<std::vector<size_t>> graph;
    std::vector<Edge> edges;

    Queue<size_t> queue;
    std::vector<size_t> pointer;
    std::vector<T> dist;
    std::vector<bool> used;
};
