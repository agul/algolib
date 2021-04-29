#pragma once
#include <algorithm>
#include <cstddef>
#include <numeric>
#include <vector>

#include "dsu.hpp"
#include "undirected_graph.hpp"

template<typename T>
struct MinimalSpanningTree {
    using weight_type = T;
    using mask_type = uint32_t;

    using edge_id_type = std::size_t;

    template<mask_type MASK, mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    weight_type operator()(const Graph<weight_type, MASK>& graph, std::vector<edge_id_type>* mst = nullptr) const {
        std::vector<edge_id_type> graph_edges(graph.edges_count());
        std::iota(graph_edges.begin(), graph_edges.end(), 0);
        std::sort(graph_edges.begin(), graph_edges.end(), [&graph](const edge_id_type& lhs, const edge_id_type& rhs) {
            return graph.weight(lhs) < graph.weight(rhs);
        });
        DSU dsu(graph.vertices_count());
        weight_type total_weight = 0;
        std::vector<edge_id_type> tree;
        for (const auto& it : graph_edges) {
            if (dsu.unite(graph.from(it), graph.to(it))) {
                total_weight += graph.weight(it);
                tree.emplace_back(it);
            }
        }
        if (mst != nullptr) {
            mst->swap(tree);
        }
        return total_weight;
    }
};
