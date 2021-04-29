#pragma once
#include <cstddef>
#include <vector>

#include "directed_graph.hpp"
#include "collections/queue/queue.hpp"

struct StronglyConnectedComponents {
    using mask_type = uint32_t;
    using size_type = std::size_t;
    using vertex_id_type = std::size_t;

    template<typename T, mask_type MASK>
    size_type operator()(const DirectedGraph<T, MASK>& graph, std::vector<vertex_id_type>* vertex_color = nullptr) const {
        const size_type vertices_count = graph.vertices_count();
        const DirectedGraph<T, MASK> reversed = graph.reversed();

        std::vector<vertex_id_type> order(vertices_count);
        graph.top_sort_rec(&order);
        std::vector<bool> used(vertices_count, false);
        std::vector<vertex_id_type> color(vertices_count);
        Queue<vertex_id_type> queue(vertices_count);
        size_type components_count = 0;
        for (const auto& v : order) {
            if (!used[v]) {
                queue.clear();
                queue.push(v);
                while (!queue.empty()) {
                    const vertex_id_type vertex = queue.pop_front();
                    color[vertex] = components_count;
                    used[vertex] = true;
                    for (const auto& it : reversed.edges(vertex)) {
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
};
