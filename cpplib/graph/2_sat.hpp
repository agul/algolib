#pragma once
#include <functional>
#include <limits>
#include <vector>

#include "directed_graph.hpp"

struct Solve2SAT {
    using mask_type = uint32_t;
    using vertex_id_type = std::size_t;
    using size_type = std::size_t;

    static constexpr vertex_id_type kUndefinedComponentId = std::numeric_limits<vertex_id_type>::max();

    template<typename T, mask_type MASK>
    bool operator()(const DirectedGraph<T, MASK>& graph, std::vector<vertex_id_type>* component_id = nullptr) const
    // requires graph_ with 2N vertices:
    // each initial vertex should be duplicated as v -> (v * 2, v * 2 + 1) as (v, !v)
    // returns false if the 2-SAT problem has no solution
    {
        std::vector<vertex_id_type> order;
        graph.top_sort_rec(&order);

        std::vector<vertex_id_type> component(graph.vertices_count(), kUndefinedComponentId);
        const auto reversed = graph.reversed();
        std::function<void(vertex_id_type, vertex_id_type)> dfs = [&reversed, &dfs, &component](const vertex_id_type v, const vertex_id_type component_id) {
            component[v] = component_id;
            for (const auto& edge : reversed.edges(v)) {
                const vertex_id_type to = edge.to();
                if (component[to] == kUndefinedComponentId) {
                    dfs(to, component_id);
                }
            }
        };

        size_type components_count = 0;
        for (const vertex_id_type v : order) {
            if (component[v] == kUndefinedComponentId) {
                dfs(v, components_count++);
            }
        }

        bool result = true;
        for (const vertex_id_type v : graph.vertices()) {
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
};
