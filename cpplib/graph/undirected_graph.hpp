#pragma once
#include <type_traits>

#include "dsu.hpp"
#include "graph.hpp"

template<typename T = int64_t, uint32_t MASK = 0>
class UndirectedGraph : public Graph<T, MASK> {
public:
    using base_graph_type = Graph<T, MASK>;
    using weight_type = typename base_graph_type::weight_type;
    using vertex_id_type = typename base_graph_type::vertex_id_type;
    using edge_id_type = typename base_graph_type::edge_id_type;
    using size_type = typename base_graph_type::size_type;
    using mask_type = typename base_graph_type::mask_type;

    UndirectedGraph() : UndirectedGraph(0) {}

    explicit UndirectedGraph(const size_type vertices_count) : base_graph_type(vertices_count) {}

    [[nodiscard]] bool is_directed() const override {
        return false;
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<!is_weighted_v<Mask>>* = nullptr>
    void add_bidirectional_edge(const vertex_id_type from, const vertex_id_type to) {
        this->add_directed_edge(from, to);
        this->add_directed_edge(to, from);
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    void add_bidirectional_edge(const vertex_id_type from, const vertex_id_type to, const weight_type weight) {
        this->add_directed_edge(from, to, weight);
        this->add_directed_edge(to, from, weight);
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<!is_weighted_v<Mask>>* = nullptr>
    void add_bidirectional_edge(const typename base_graph_type::Edge& edge) {
        add_bidirectional_edge(edge.from(), edge.to());
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    void add_bidirectional_edge(const typename base_graph_type::Edge& edge) {
        add_bidirectional_edge(edge.from(), edge.to(), edge.weight());
    }

    void remove_last_bidirectional_edge() {
        this->remove_last_directed_edge();
        this->remove_last_directed_edge();  // each bidirectional edge is added twice
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    weight_type minimal_spanning_tree(std::vector<vertex_id_type>* mst = nullptr) const {
        std::vector<edge_id_type> graph_edges(this->edges_count());
        std::iota(graph_edges.begin(), graph_edges.end(), 0);
        std::sort(graph_edges.begin(), graph_edges.end(), [this](const edge_id_type& lhs, const edge_id_type& rhs) {
            return this->weight(lhs) < this->weight(rhs);
        });
        DSU dsu(this->vertices_count_);
        weight_type total_weight = 0;
        std::vector<vertex_id_type> tree;
        for (const auto& it : graph_edges) {
            if (dsu.unite(this->from(it), this->to(it))) {
                total_weight += this->weight(it);
                tree.emplace_back(it);
            }
        }
        if (mst != nullptr) {
            mst->swap(tree);
        }
        return total_weight;
    }

    [[nodiscard]] bool is_connected() const {
        return dsu().sets_count() == 1;
    }

    [[nodiscard]] DSU dsu() const {
        DSU dsu(this->vertices_count_);
        for (const auto& it : this->edges()) {
            dsu.unite(it.from(), it.to());
        }
        return dsu;
    }

    [[nodiscard]] DSU::container_type labelled_components() const {
        return dsu().finalize().data();
    }
};
