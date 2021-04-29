#pragma once
#include <algorithm>
#include <cstddef>
#include <vector>

#include "graph.hpp"
#include "collections/stack/stack.hpp"

struct EulerianPath {
    using mask_type = uint32_t;
    using vertex_id_type = std::size_t;
    using edge_id_type = std::size_t;
    using size_type = std::size_t;

    template<typename T, mask_type MASK>
    bool operator()(
            const Graph<T, MASK>& graph,
            std::vector<edge_id_type>* eulerian_path = nullptr,
            const bool strict_cycle = false,
            const vertex_id_type starting_vertex = 0,
            const bool strict_starting_vertex = false) const {
        using graph_type = Graph<T, MASK>;

        const size_type vertices_count = graph.vertices_count();
        const size_type edges_count = graph.edges_count();
        const bool is_directed = graph.is_directed();

        const size_type real_edges_count = edges_count / (is_directed ? 1 : 2);
        if (real_edges_count == 0) {
            if (eulerian_path != nullptr) {
                eulerian_path->clear();
            }
            return true;
        }
        std::vector<size_type> inbound_degree(vertices_count, 0);
        std::vector<size_type> outbound_degree(vertices_count, 0);
        for (const auto& it : graph.edges()) {
            ++outbound_degree[it.from()];
            if (is_directed) {
                ++inbound_degree[it.to()];  // in undirected graph_ all edges_ are duplicated: forward and backward
            }
        }
        std::vector<vertex_id_type> odd_degree_vertices;
        for (const vertex_id_type v : graph.vertices()) {
            if ((inbound_degree[v] + outbound_degree[v]) % 2 == 1) {
                odd_degree_vertices.emplace_back(v);
            }
        }
        if (is_directed) {
            int32_t min_diff = 0;
            int32_t max_diff = 0;
            size_type not_equal_degree_vertices_count = 0;
            for (const vertex_id_type v : graph.vertices()) {
                const int32_t diff = static_cast<int32_t>(outbound_degree[v]) - inbound_degree[v];
                if (diff != 0) {
                    ++not_equal_degree_vertices_count;
                    min_diff = std::min(min_diff, diff);
                    max_diff = std::max(max_diff, diff);
                }
            }
            if (not_equal_degree_vertices_count > 0 && (not_equal_degree_vertices_count != 2 || min_diff != -1 || max_diff != 1)) {
                return false;
            }
        }
        vertex_id_type root = starting_vertex;
        if (!odd_degree_vertices.empty()) {
            if (odd_degree_vertices.size() > 2 || strict_cycle) {
                return false;
            }
            const bool starting_vertex_has_odd_degree = (std::find(odd_degree_vertices.cbegin(), odd_degree_vertices.cend(), starting_vertex) != odd_degree_vertices.cend());
            if (!starting_vertex_has_odd_degree || outbound_degree[starting_vertex] < inbound_degree[starting_vertex]) {
                if (strict_starting_vertex) {
                    return false;
                }
                for (const vertex_id_type v : odd_degree_vertices) {
                    if (outbound_degree[v] > inbound_degree[v]) {
                        root = v;
                        break;
                    }
                }
            }
        }
        else if (outbound_degree[root] == 0) {
            if (strict_starting_vertex) {
                return false;
            }
            for (const vertex_id_type v : graph.vertices()) {
                if (outbound_degree[v] > 0) {
                    root = v;
                }
            }
        }

        std::vector<bool> used_edge(edges_count, false);
        auto mark_used = [&used_edge, &is_directed](const edge_id_type edge_id) {
            used_edge[edge_id] = true;
            if (!is_directed) {
                used_edge[edge_id ^ 1] = true;  // mark reversed edge as directed
            }
        };

        std::vector<typename graph_type::EdgesList::const_iterator> current_edge_pointer(vertices_count);
        for (const vertex_id_type v : graph.vertices()) {
            current_edge_pointer[v] = graph.edges_list(v).cbegin();
        }
        const edge_id_type kFakeEdgeId = std::numeric_limits<edge_id_type>::max();
        const size_type max_edges_count = real_edges_count + 1;  // add fake edge
        Stack<std::pair<vertex_id_type, edge_id_type>> stack(max_edges_count);
        stack.push({root, kFakeEdgeId});

        std::vector<edge_id_type> result;
        result.reserve(max_edges_count);
        while (!stack.empty()) {
            vertex_id_type v;
            edge_id_type last_edge_id;
            std::tie(v, last_edge_id) = stack.top();

            auto& iterator = current_edge_pointer[v];
            if (iterator == graph.edges_list(v).cend()) {
                result.emplace_back(last_edge_id);
                stack.pop();
                continue;
            }

            const edge_id_type next_edge_id = *iterator++;
            if (used_edge[next_edge_id]) {
                continue;
            }
            mark_used(next_edge_id);
            stack.push({graph.to(next_edge_id), next_edge_id});
        }
        if (!result.empty() && result.back() == kFakeEdgeId) {
            result.pop_back();
        }
        if (result.size() != real_edges_count) {
            return false;
        }

        if (eulerian_path != nullptr) {
            std::reverse(result.begin(), result.end());
            eulerian_path->swap(result);
        }
        return true;
    }
};
