#pragma once
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

    bool solve_2_sat(std::vector<vertex_id_type>* component_id = nullptr) const;

    [[nodiscard]] DirectedGraph reversed() const;

    size_type scc(std::vector<vertex_id_type>* vertex_color = nullptr) const;

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
bool DirectedGraph<T, MASK>::solve_2_sat(std::vector<vertex_id_type>* component_id) const
// requires graph with 2N vertices:
// each initial vertex should be duplicated as v -> (v * 2, v * 2 + 1) as (v, !v)
// returns false if the 2-SAT problem has no solution
{
    std::vector<vertex_id_type> order;
    this->top_sort_rec(&order);

    std::vector<vertex_id_type> component(this->vertices_count_, std::numeric_limits<vertex_id_type>::max());
    const auto reversed = this->reversed();
    std::function<void(vertex_id_type, vertex_id_type)> dfs = [&reversed, &dfs, &component](const vertex_id_type v, const vertex_id_type component_id) {
        component[v] = component_id;
        for (const auto& edge : reversed.edges(v)) {
            const vertex_id_type to = edge.to();
            if (component[to] == std::numeric_limits<vertex_id_type>::max()) {
                dfs(to, component_id);
            }
        }
    };

    size_type components_count = 0;
    for (const vertex_id_type v : order) {
        if (component[v] == std::numeric_limits<vertex_id_type>::max()) {
            dfs(v, components_count++);
        }
    }

    bool result = true;
    for (const vertex_id_type v : this->vertices()) {
        if (component[v] == component[v ^ 1]) {
            result = false;
            break;
        }
    }
    if (component_id != nullptr) {
        component_id->swap(component);
    }
    return result;
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

template<typename T, uint32_t MASK>
typename DirectedGraph<T, MASK>::size_type DirectedGraph<T, MASK>::scc(std::vector<vertex_id_type>* vertex_color) const {
    const DirectedGraph<T, MASK> rev_graph = reversed();
    std::vector<vertex_id_type> order(this->vertices_count_);
    top_sort_rec(&order);
    std::vector<bool> used(this->vertices_count_, false);
    std::vector<vertex_id_type> color(this->vertices_count_);
    Queue<vertex_id_type> queue(this->vertices_count_);
    size_type components_count = 0;
    for (const auto& v : order) {
        if (!used[v]) {
            queue.clear();
            queue.push(v);
            while (!queue.empty()) {
                const vertex_id_type vertex = queue.pop_front();
                color[vertex] = components_count;
                used[vertex] = true;
                for (const auto& it : rev_graph.edges(vertex)) {
                    const vertex_id_type to = it.to();
                    if (!used[to]) {
                        used[to] = true;
                        queue.push(to);
                    }
                }
            }
            ++components_count;
        }
    }
    if (vertex_color != nullptr) {
        vertex_color->swap(color);
    }
    return components_count;
}
