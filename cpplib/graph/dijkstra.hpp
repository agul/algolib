#pragma once
#include <cstddef>
#include <functional>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

#include "graph.hpp"
#include "base/helpers.hpp"

template<typename T>
class Dijkstra {
public:
    using weight_type = T;
    using mask_type = uint32_t;
    using vertex_id_type = std::size_t;
    using edge_id_type = std::size_t;
    using size_type = std::size_t;

    static constexpr edge_id_type kUndefinedEdgeId = std::numeric_limits<edge_id_type>::max();

    template<mask_type MASK, mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    Dijkstra(const Graph<weight_type, MASK>& graph, const vertex_id_type start_vertex) :
            distance_(graph.vertices_count(), graph.weight_infinity()),
            last_edge_(graph.vertices_count(), kUndefinedEdgeId),
            start_vertex_(start_vertex)
    {
        if (graph.is_sparse()) {
            sparse_dijkstra(graph);
        } else {
            dense_dijkstra(graph);
        }
    }

    [[nodiscard]] const std::vector<weight_type>& distance() const {
        return distance_;
    }

    [[nodiscard]] const std::vector<edge_id_type>& last_edge() const {
        return last_edge_;
    }

    template<mask_type MASK, mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    weight_type shortest_path(const Graph<weight_type, MASK>& graph, const vertex_id_type finish_vertex, std::vector<edge_id_type>* path = nullptr) const {
        if (start_vertex_ == finish_vertex) {
            return 0;
        }
        if (path != nullptr) {
            std::vector<edge_id_type> tmp_path;
            vertex_id_type vertex = finish_vertex;
            while (vertex != start_vertex_) {
                const edge_id_type edge = last_edge_[vertex];
                tmp_path.emplace_back(edge);
                vertex = graph.from(edge);
            }
            std::reverse(tmp_path.begin(), tmp_path.end());
            path->swap(tmp_path);
        }
        return distance_[finish_vertex];
    }

private:
    std::vector<weight_type> distance_;
    std::vector<edge_id_type> last_edge_;
    vertex_id_type start_vertex_;

    template<mask_type MASK, mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    void sparse_dijkstra(const Graph<weight_type, MASK>& graph) {
        distance_[start_vertex_] = 0;
        using QueueNode = std::pair<weight_type, vertex_id_type>;
        std::priority_queue<QueueNode, std::vector<QueueNode>, std::greater<QueueNode>> q;
        q.emplace(0, start_vertex_);
        while (!q.empty()) {
            const QueueNode node = q.top();
            q.pop();
            const weight_type len = node.first;
            const vertex_id_type vertex = node.second;
            if (len > distance_[vertex]) {
                continue;
            }
            for (const auto& it : graph.edges(vertex)) {
                const weight_type new_dist = len + it.weight();
                const vertex_id_type to = it.to();
                if (umin(distance_[to], new_dist)) {
                    q.push(std::make_pair(new_dist, to));
                    last_edge_[to] = it.id();
                }
            }
        }
    }

    template<mask_type MASK, mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    void dense_dijkstra(const Graph<weight_type, MASK>& graph) {
        static constexpr vertex_id_type kUndefinedVertex = std::numeric_limits<vertex_id_type>::max();
        std::vector<bool> used(graph.vertices_count(), false);
        distance_[start_vertex_] = 0;
        for (size_type iter = 0; iter < graph.vertices_count(); ++iter) {
            vertex_id_type vertex = kUndefinedVertex;
            weight_type min_dist = graph.weight_infinity();
            for (const vertex_id_type v : graph.vertices()) {
                if (!used[v] && umin(min_dist, distance_[v])) {
                    min_dist = distance_[v];
                    vertex = v;
                }
            }
            if (vertex == kUndefinedVertex) {
                break;
            }
            used[vertex] = true;
            for (const auto& it : graph.edges(vertex)) {
                const vertex_id_type to = it.to();
                const weight_type len = min_dist + it.weight();
                if (!used[to] && umin(distance_[to], len)) {
                    distance_[to] = len;
                    last_edge_[to] = it.id();
                }
            }
        }
    }
};
