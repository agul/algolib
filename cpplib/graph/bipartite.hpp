#pragma once
#include <cstddef>
#include <limits>
#include <vector>

#include "graph.hpp"
#include "collections/queue/queue.hpp"

struct IsBipartite {
    using mask_type = uint32_t;
    using vertex_id_type = std::size_t;
    using size_type = std::size_t;

    static constexpr vertex_id_type kUndefinedPart = std::numeric_limits<vertex_id_type>::max();

    template<typename T, mask_type MASK>
    bool operator()(const Graph<T, MASK>& graph, std::vector<vertex_id_type>* partition = nullptr) {
        const size_type vertices_count = graph.vertices_count();

        std::vector<vertex_id_type> part(vertices_count, kUndefinedPart);
        Queue<vertex_id_type> q(vertices_count);
        for (size_type i = 0; i < vertices_count; ++i) {
            if (part[i] == kUndefinedPart) {
                q.clear();
                q.push(i);
                part[i] = 0;
                while (!q.empty()) {
                    const vertex_id_type vertex = q.pop_front();
                    const vertex_id_type color = part[vertex];
                    for (const auto& it : graph.edges(vertex)) {
                        const vertex_id_type to = it.to();
                        if (part[to] == kUndefinedPart) {
                            part[to] = color ^ 1;
                            q.push(to);
                            continue;
                        }
                        if (part[to] == color) {
                            return false;
                        }
                    }
                }
            }
        }
        if (partition != nullptr) {
            partition->swap(part);
        }
        return true;
    }
};
