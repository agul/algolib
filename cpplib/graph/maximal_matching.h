#pragma once
#include <limits>
#include <vector>

#include "graph.hpp"

struct MaximalMatching {
public:
    using mask_type = uint32_t;
    using vertex_id_type = std::size_t;
    using size_type = std::size_t;

    static constexpr vertex_id_type kNotMatchedVertexId = std::numeric_limits<vertex_id_type>::max();

    template<typename T, mask_type MASK>
    explicit MaximalMatching(const Graph<T, MASK>& graph) :
            vertices_count_(graph.vertices_count()),
            match_(vertices_count_, kNotMatchedVertexId)
    {
        build_matching(graph);
    }

    [[nodiscard]] const std::vector<vertex_id_type>& matching() const {
        return match_;
    }

private:
    size_type vertices_count_;
    std::vector<vertex_id_type> match_;
    std::vector<bool> used_;

    template<typename T, mask_type MASK>
    void build_matching(const Graph<T, MASK>& graph) {
        std::vector<bool> matched(vertices_count_, false);
        for (const vertex_id_type v : graph.vertices()) {
            for (const auto& it : graph.edges(v)) {
                const vertex_id_type to = it.to();
                if (match_[to] == kNotMatchedVertexId) {
                    match_[to] = v;
                    matched[v] = true;
                    break;
                }
            }
        }
        for (const vertex_id_type v : graph.vertices()) {
            if (matched[v]) {
                continue;
            }
            used_.assign(vertices_count_, false);
            try_kuhn(graph, v);
        }
    }

    template<typename T, mask_type MASK>
    bool try_kuhn(const Graph<T, MASK>& graph, const vertex_id_type vertex) {
        used_[vertex] = true;
        for (const auto& it : graph.edges(vertex)) {
            const vertex_id_type to = it.to();
            if (match_[to] == kNotMatchedVertexId) {
                match_[to] = vertex;
                match_[vertex] = to;
                return true;
            }
        }
        for (const auto& it : graph.edges(vertex)) {
            const vertex_id_type to = it.to();
            if (!used_[match_[to]] && try_kuhn(graph, match_[to])) {
                match_[to] = vertex;
                match_[vertex] = to;
                return true;
            }
        }
        return false;
    }
};