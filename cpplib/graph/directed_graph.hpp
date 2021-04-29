#pragma once
#include <algorithm>
#include <vector>

#include "graph.hpp"

template<typename T = int64_t, uint32_t MASK = 0>
class DirectedGraph : public Graph<T, MASK> {
public:
    using base_graph_type = Graph<T, MASK>;
    using vertex_id_type = typename base_graph_type::vertex_id_type;
    using size_type = typename base_graph_type::size_type;

    DirectedGraph() : DirectedGraph(0) {}

    explicit DirectedGraph(const size_type vertices_count) : base_graph_type(vertices_count) {}

    [[nodiscard]] bool is_acyclic() const;
    bool top_sort_acyclic(std::vector<vertex_id_type>* vertex_order = nullptr) const;
    void top_sort_rec(std::vector<vertex_id_type>* vertex_order) const;

    [[nodiscard]] DirectedGraph reversed() const;

private:
    void top_sort_rec_impl(vertex_id_type vertex, std::vector<vertex_id_type>& order, std::vector<bool>& used) const;
};

template<typename T, uint32_t MASK>
DirectedGraph<T, MASK> DirectedGraph<T, MASK>::reversed() const {
    DirectedGraph<T, MASK> result(this->vertices_count_);
    for (const auto& edge : this->edges()) {
        result.add_directed_edge(edge.reversed());
    }
    return result;
}

template<typename T, uint32_t MASK>
bool DirectedGraph<T, MASK>::is_acyclic() const {
    return this->top_sort_acyclic();
}

template<typename T, uint32_t MASK>
bool DirectedGraph<T, MASK>::top_sort_acyclic(std::vector<vertex_id_type>* vertex_order) const
// non-recursive topological sorting, works only for acyclic graphs
{
    std::vector<vertex_id_type> order;
    std::vector<size_type> inbound_degree(this->vertices_count_, 0);
    for (const auto& to : this->to_) {
        ++inbound_degree[to];
    }
    for (const vertex_id_type v : this->vertices()) {
        if (inbound_degree[v] == 0) {
            order.emplace_back(v);
        }
    }
    size_type head = 0;
    while (head < order.size()) {
        const vertex_id_type cur_vertex = order[head++];
        for (const auto& it : this->edges(cur_vertex)) {
            const vertex_id_type to = it.to();
            --inbound_degree[to];
            if (inbound_degree[to] == 0) {
                order.emplace_back(to);
            }
        }
    }
    const bool result = (order.size() == this->vertices_count_);
    if (vertex_order != nullptr) {
        vertex_order->swap(order);
    }
    return result;
}

template<typename T, uint32_t MASK>
void DirectedGraph<T, MASK>::top_sort_rec(std::vector<vertex_id_type>* vertex_order) const {
    std::vector<bool> used(this->vertices_count_, false);
    std::vector<vertex_id_type> order;
    for (const vertex_id_type v : this->vertices()) {
        if (!used[v]) {
            top_sort_rec_impl(v, order, used);
        }
    }
    std::reverse(order.begin(), order.end());
    vertex_order->swap(order);
}

template<typename T, uint32_t MASK>
void DirectedGraph<T, MASK>::top_sort_rec_impl(const vertex_id_type vertex, std::vector<vertex_id_type>& order, std::vector<bool>& used) const {
    used[vertex] = true;
    for (const auto& it : this->edges(vertex)) {
        const vertex_id_type to = it.to();
        if (!used[to]) {
            top_sort_rec_impl(to, order, used);
        }
    }
    order.emplace_back(vertex);
}
