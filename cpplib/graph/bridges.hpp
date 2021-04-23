#pragma once
#include <cstddef>
#include <vector>

#include "undirected_graph.hpp"

class GraphBridges {
public:
    using vertex_id_type = std::size_t;
    using edge_id_type = std::size_t;
    using timer_type = std::size_t;
    using mask_type = uint32_t;

    template<typename T, mask_type MASK>
    explicit GraphBridges(const UndirectedGraph<T, MASK>& graph) :
            fup_(graph.vertices_count()),
            tin_(graph.vertices_count()),
            used_(graph.vertices_count()),
            timer_(0)
    {
        find_bridges(graph);
    }

    [[nodiscard]] const std::vector<edge_id_type>& bridges() const {
        return bridges_;
    }

private:
    template<typename T, mask_type MASK>
    void find_bridges(const UndirectedGraph<T, MASK>& graph) {
        for (const vertex_id_type v : graph.vertices()) {
            if (!used_[v]) {
                dfs(graph, v, 0, true);
            }
        }
    }

    template<typename T, mask_type MASK>
    void dfs(const UndirectedGraph<T, MASK>& graph, const vertex_id_type vertex, const edge_id_type prev_edge, const bool first) {
        used_[vertex] = true;
        tin_[vertex] = timer_;
        fup_[vertex] = timer_;
        ++timer_;
        for (const auto& it : graph.edges(vertex)) {
            if (!first && it.id() == (prev_edge ^ 1)) {
                continue;
            }
            const vertex_id_type to = it.to();
            if (used_[to]) {
                umin(fup_[vertex], tin_[to]);
            }
            else {
                dfs(graph, to, it.id(), false);
                umin(fup_[vertex], fup_[to]);
                if (fup_[to] > tin_[vertex]) {
                    bridges_.emplace_back(it.id());
                }
            }
        }
    }

    std::vector<edge_id_type> bridges_;
    std::vector<timer_type> fup_;
    std::vector<timer_type> tin_;
    std::vector<bool> used_;
    timer_type timer_;
};
