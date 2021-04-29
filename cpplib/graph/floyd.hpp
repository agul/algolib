#pragma once
#include <cstddef>
#include <vector>

#include "graph.hpp"
#include "base/helpers.hpp"

template<typename T>
struct Floyd {
    using weight_type = T;
    using mask_type = uint32_t;

    template<mask_type MASK, mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    std::vector<std::vector<weight_type>> operator()(const Graph<T, MASK>& graph) const {
        auto dist = make_vector<weight_type>(graph.vertices_count(), graph.vertices_count(), graph.weight_infinity());
        for (const auto v : graph.vertices()) {
            dist[v][v] = 0;
        }
        for (const auto& edge : graph.edges()) {
            umin(dist[edge.from()][edge.to()], edge.weight());
        }
        for (const auto k : graph.vertices()) {
            for (const auto i : graph.vertices()) {
                for (const auto j : graph.vertices()) {
                    umin(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
        return dist;
    }
};
