#pragma once
#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <vector>

#include "base/helpers.hpp"
#include "maths/maths.hpp"
#include "range/ranges.hpp"
#include "collections/queue/queue.hpp"
#include "collections/stack/stack.hpp"

enum GraphType {
    Weighted = 1
};

template<uint32_t MASK>
struct is_weighted
{
    /// caide keep
    static constexpr bool value = (MASK & GraphType::Weighted) != 0;
};

template<uint32_t MASK>
constexpr bool is_weighted_v = is_weighted<MASK>::value;

template<typename T = int64_t, uint32_t MASK = 0>
class Graph {
public:
    using size_type = std::size_t;
    using vertex_id_type = std::size_t;
    using edge_id_type = std::size_t;
    using weight_type = T;
    using mask_type = uint32_t;

    using EdgesList = std::vector<edge_id_type>;

    class Edge {
    public:
        constexpr Edge(
                const std::vector<vertex_id_type>& from,
                const std::vector<vertex_id_type>& to,
                const std::vector<weight_type>& weight,
                const edge_id_type index
        ) :
                from_(from), to_(to), weight_(weight), index_(index)
        {}

        [[nodiscard]] vertex_id_type from() const {
            return from_[index_];
        }

        [[nodiscard]] vertex_id_type to() const {
            return to_[index_];
        }

        template<mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
        [[nodiscard]] weight_type weight() const {
            return weight_[index_];
        }

        [[nodiscard]] constexpr edge_id_type id() const {
            return index_;
        }

        void set_index(const edge_id_type index) {
            index_ = index;
        }

        [[nodiscard]] constexpr Edge reversed() const {
            return Edge(to_, from_, weight_, index_);
        }

    private:
        const std::vector<vertex_id_type>& from_;
        const std::vector<vertex_id_type>& to_;
        const std::vector<weight_type>& weight_;
        edge_id_type index_;
    };

    template<typename Range>
    class EdgesHolder {
    public:
        template<typename BaseConstIterator>
        class EdgeConstIterator : public BaseConstIterator {
        public:
            using value_type = Edge;
            using pointer = const value_type*;
            using const_reference = const value_type&;

            constexpr explicit EdgeConstIterator(
                    BaseConstIterator it,
                    const std::vector<vertex_id_type>& from,
                    const std::vector<vertex_id_type>& to,
                    const std::vector<weight_type>& weight
            ) :
                    BaseConstIterator(it),
                    cur_edge_(from, to, weight, 0)
            {}

            pointer operator->() {
                return addressof(operator*());
            }

            const_reference operator*() {
                const typename BaseConstIterator::value_type index = this->BaseConstIterator::operator*();
                cur_edge_.set_index(index);
                return cur_edge_;
            }

        private:
            Edge cur_edge_;
        };

        using const_iterator = EdgeConstIterator<typename Range::const_iterator>;
        using value_type = typename const_iterator::value_type;

        constexpr EdgesHolder(
                const Range& range,
                const std::vector<vertex_id_type>& from,
                const std::vector<vertex_id_type>& to,
                const std::vector<weight_type>& weight
        ) :
                begin_(range.begin(), from, to, weight),
                end_(range.end(), from, to, weight)
        {}

        [[nodiscard]] constexpr const_iterator begin() const {
            return begin_;
        }

        [[nodiscard]] constexpr const_iterator end() const {
            return end_;
        }

        [[nodiscard]] constexpr size_type size() const {
            return end_ - begin_;
        }

    private:
        const const_iterator begin_;
        const const_iterator end_;
    };

    Graph() : Graph(0) {}

    explicit Graph(const size_type vertices_count) {
        init(vertices_count);
    }

    virtual ~Graph() {
        clear();
    }

    [[nodiscard]] virtual bool is_directed() const {
        return true;
    }

    void init(const size_type vertices_count) {
        clear();
        vertices_count_ = vertices_count;
        edges_.resize(vertices_count_);
    }

    [[nodiscard]] IntegerRange<vertex_id_type> vertices() const {
        return range(vertices_count_);
    }

    [[nodiscard]] IntegerRange<vertex_id_type>::const_iterator begin() const {
        return vertices().begin();
    }

    [[nodiscard]] IntegerRange<vertex_id_type>::const_iterator end() const {
        return vertices().end();
    }

    EdgesHolder<EdgesList> edges(const vertex_id_type vertex) const {
        return EdgesHolder<EdgesList>(edges_[vertex], from_, to_, weight_);
    }

    EdgesHolder<IntegerRange<edge_id_type>> edges() const {
        return EdgesHolder<IntegerRange<edge_id_type>>(range(from_.size()), from_, to_, weight_);
    }

    [[nodiscard]] const std::vector<edge_id_type>& edges_list(const vertex_id_type vertex) const {
        return edges_[vertex];
    }

    void clear();

    [[nodiscard]] size_type vertices_count() const {
        return vertices_count_;
    }

    [[nodiscard]] size_type edges_count() const {
        return from_.size();
    }

    [[nodiscard]] vertex_id_type from(const edge_id_type index) const {
        return from_[index];
    }

    [[nodiscard]] vertex_id_type to(const edge_id_type index) const {
        return to_[index];
    }

    [[nodiscard]] Edge operator [](const edge_id_type index) const {
        return Edge(from_, to_, weight_, index);
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    [[nodiscard]] weight_type weight(const edge_id_type index) const {
        return weight_[index];
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    static weight_type weight_infinity() {
        return std::numeric_limits<weight_type>::max() / 2;
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<!is_weighted_v<Mask>>* = nullptr>
    void add_directed_edge(const vertex_id_type from, const vertex_id_type to) {
        push_edge(from, to);
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    void add_directed_edge(const vertex_id_type from, const vertex_id_type to, const weight_type weight) {
        weight_.emplace_back(weight);
        push_edge(from, to);
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<!is_weighted_v<Mask>>* = nullptr>
    void remove_last_directed_edge() {
        pop_edge();
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    void remove_last_directed_edge() {
        weight_.pop_back();
        pop_edge();
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<!is_weighted_v<Mask>>* = nullptr>
    void add_directed_edge(const Edge& edge) {
        add_directed_edge(edge.from(), edge.to());
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    void add_directed_edge(const Edge& edge) {
        add_directed_edge(edge.from(), edge.to(), edge.weight());
    }

    [[nodiscard]] bool is_sparse() const {
        return vertices_count_ == 0 || sqr<int64_t>(vertices_count_) >= (edges_count() << 4);
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    [[nodiscard]] std::vector<std::vector<weight_type>> floyd() const {
        auto dist = make_vector<weight_type>(vertices_count_, vertices_count_, weight_infinity());
        for (const auto v : vertices()) {
            dist[v][v] = 0;
        }
        for (const auto& edge : edges()) {
            umin(dist[edge.from()][edge.to()], edge.weight());
        }
        for (const auto k : vertices()) {
            for (const auto i : vertices()) {
                for (const auto j : vertices()) {
                    umin(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
        return dist;
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    void dijkstra(const vertex_id_type start_vertex, std::vector<weight_type>* distance, std::vector<edge_id_type>* last_edge = nullptr) const {
        if (is_sparse()) {
            sparse_dijkstra(start_vertex, distance, last_edge);
        } else {
            dense_dijkstra(start_vertex, distance, last_edge);
        }
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    weight_type dijkstra(const vertex_id_type start_vertex, const vertex_id_type finish_vertex, std::vector<edge_id_type>* path = nullptr) const {
        if (start_vertex == finish_vertex) {
            return 0;
        }
        std::vector<weight_type> dist;
        std::vector<edge_id_type> last_edge;
        dijkstra(start_vertex, &dist, &last_edge);
        if (path != nullptr) {
            std::vector<edge_id_type> tmp_path;
            vertex_id_type vertex = finish_vertex;
            while (vertex != start_vertex) {
                const edge_id_type edge = last_edge[vertex];
                tmp_path.emplace_back(edge);
                vertex = from_[edge];
            }
            std::reverse(tmp_path.begin(), tmp_path.end());
            path->swap(tmp_path);
        }
        return dist[finish_vertex];
    }

    [[nodiscard]] vertex_id_type find_vertex_with_max_degree() const;

    bool find_eulerian_path(std::vector<edge_id_type>* eulerian_path = nullptr, bool strict_cycle = false, vertex_id_type starting_vertex = 0, bool strict_starting_vertex = false);

    bool is_bipartite(std::vector<vertex_id_type>& partition) const;
    void maximal_matching(std::vector<vertex_id_type>* match) const;

    bool try_kuhn(vertex_id_type vertex, std::vector<bool>& used, std::vector<vertex_id_type>& match) const;

protected:
    void push_edge(const vertex_id_type from, const vertex_id_type to) {
        const edge_id_type edge_id = from_.size();
        from_.emplace_back(from);
        to_.emplace_back(to);
        edges_[from].emplace_back(edge_id);
    }

    void pop_edge() {
        const vertex_id_type from = from_.back();
        from_.pop_back();
        to_.pop_back();
        edges_[from].pop_back();
    }

    std::vector<EdgesList> edges_;
    std::vector<vertex_id_type> from_;
    std::vector<vertex_id_type> to_;
    std::vector<weight_type> weight_;

    size_type vertices_count_;

private:
    template<mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    void sparse_dijkstra(const vertex_id_type start_vertex, std::vector<weight_type>* distance, std::vector<edge_id_type>* last_edge) const {
        std::vector<weight_type> dist(vertices_count_, weight_infinity());
        std::vector<edge_id_type> last(vertices_count_, std::numeric_limits<edge_id_type>::max());
        dist[start_vertex] = 0;
        using QueueNode = std::pair<weight_type, vertex_id_type>;
        std::priority_queue<QueueNode> q;
        q.emplace(0, start_vertex);
        while (!q.empty()) {
            const QueueNode node = q.top();
            q.pop();
            const weight_type len = -node.first;
            const vertex_id_type vertex = node.second;
            if (len > dist[vertex]) {
                continue;
            }
            for (const auto& it : edges(vertex)) {
                const weight_type new_dist = len + it.weight();
                const vertex_id_type to = it.to();
                if (umin(dist[to], new_dist)) {
                    q.push(std::make_pair(-new_dist, to));
                    last[to] = it.id();
                }
            }
        }
        distance->swap(dist);
        if (last_edge != nullptr) {
            last_edge->swap(last);
        }
    }

    template<mask_type Mask = MASK, typename std::enable_if_t<is_weighted_v<Mask>>* = nullptr>
    void dense_dijkstra(const vertex_id_type start_vertex, std::vector<weight_type>* distance, std::vector<edge_id_type>* last_edge) const {
        std::vector<bool> used(vertices_count_);
        std::vector<weight_type> dist(vertices_count_, weight_infinity());
        std::vector<edge_id_type> last(vertices_count_, std::numeric_limits<edge_id_type>::max());
        dist[start_vertex] = 0;
        for (size_type iter = 0; iter < vertices_count_; ++iter) {
            vertex_id_type vertex = std::numeric_limits<vertex_id_type>::max();
            weight_type min_dist = weight_infinity();
            for (const vertex_id_type v : vertices()) {
                if (!used[v] && umin(min_dist, dist[v])) {
                    min_dist = dist[v];
                    vertex = v;
                }
            }
            if (vertex == std::numeric_limits<vertex_id_type>::max()) {
                break;
            }
            used[vertex] = true;
            for (const auto& it : edges(vertex)) {
                const vertex_id_type to = it.to();
                const weight_type len = min_dist + it.weight();
                if (!used[to] && umin(dist[to], len)) {
                    dist[to] = len;
                    last[to] = it.id();
                }
            }
        }
        distance->swap(dist);
        if (last_edge != nullptr) {
            last_edge->swap(last);
        }
    }

};

template<typename T, uint32_t MASK>
void Graph<T, MASK>::clear() {
    vertices_count_ = 0;
    edges_.clear();
    from_.clear();
    to_.clear();
    weight_.clear();
}

template<typename T, uint32_t MASK>
typename Graph<T, MASK>::vertex_id_type Graph<T, MASK>::find_vertex_with_max_degree() const {
    const auto iter = std::max_element(edges_.begin(), edges_.end(),
                                       [](const EdgesList& lhs, const EdgesList& rhs) {
                                           return lhs.size() < rhs.size();
                                       });
    return static_cast<vertex_id_type>(std::distance(edges_.begin(), iter));
}

template<typename T, uint32_t MASK>
bool Graph<T, MASK>::is_bipartite(std::vector<vertex_id_type>& partition) const {
    const vertex_id_type kNone = std::numeric_limits<vertex_id_type>::max();
    partition.assign(vertices_count_, kNone);
    Queue<vertex_id_type> q(vertices_count_);
    for (size_type i = 0; i < vertices_count_; ++i) {
        if (partition[i] == kNone) {
            q.clear();
            q.push(i);
            partition[i] = 0;
            while (!q.empty()) {
                const vertex_id_type vertex = q.pop_front();
                const vertex_id_type color = partition[vertex];
                for (const auto& it : edges(vertex)) {
                    const vertex_id_type to = it.to();
                    if (partition[to] == kNone) {
                        partition[to] = color ^ 1;
                        q.push(to);
                        continue;
                    }
                    if (partition[to] == color) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

template<typename T, uint32_t MASK>
void Graph<T, MASK>::maximal_matching(std::vector<vertex_id_type>* vertex_match) const {
    std::vector<vertex_id_type> match(vertices_count_, std::numeric_limits<vertex_id_type>::max());
    std::vector<bool> matched(vertices_count_, false);
    for (const vertex_id_type v : vertices()) {
        for (const auto& it : edges(v)) {
            const vertex_id_type to = it.to();
            if (match[to] == std::numeric_limits<vertex_id_type>::max()) {
                match[to] = v;
                matched[v] = true;
                break;
            }
        }
    }
    std::vector<bool> used;
    for (const vertex_id_type v : vertices()) {
        if (matched[v]) {
            continue;
        }
        used.assign(vertices_count_, false);
        try_kuhn(v, used, match);
    }
    vertex_match->swap(match);
}

template<typename T, uint32_t MASK>
bool Graph<T, MASK>::try_kuhn(const vertex_id_type vertex, std::vector<bool>& used, std::vector<vertex_id_type>& match) const {
    used[vertex] = true;
    for (const auto& it : edges(vertex)) {
        const vertex_id_type to = it.to();
        if (match[to] == std::numeric_limits<vertex_id_type>::max()) {
            match[to] = vertex;
            match[vertex] = to;
            return true;
        }
    }
    for (const auto& it : edges(vertex)) {
        const vertex_id_type to = it.to();
        if (!used[match[to]] && try_kuhn(match[to], used, match)) {
            match[to] = vertex;
            match[vertex] = to;
            return true;
        }
    }
    return false;
}

template<typename T, uint32_t MASK>
bool Graph<T, MASK>::find_eulerian_path(std::vector<edge_id_type>* eulerian_path, const bool strict_cycle, const vertex_id_type starting_vertex, const bool strict_starting_vertex) {
    const size_type real_edges_count = edges_count() / (is_directed() ? 1 : 2);
    if (real_edges_count == 0) {
        if (eulerian_path != nullptr) {
            eulerian_path->clear();
        }
        return true;
    }
    std::vector<size_type> inbound_degree(vertices_count_, 0);
    std::vector<size_type> outbound_degree(vertices_count_, 0);
    for (const auto& it : edges()) {
        ++outbound_degree[it.from()];
        if (is_directed()) {
            ++inbound_degree[it.to()];  // in undirected graph all edges are duplicated: forward and backward
        }
    }
    std::vector<vertex_id_type> odd_degree_vertices;
    for (const vertex_id_type v : vertices()) {
        if ((inbound_degree[v] + outbound_degree[v]) % 2 == 1) {
            odd_degree_vertices.emplace_back(v);
        }
    }
    if (is_directed()) {
        int32_t min_diff = 0;
        int32_t max_diff = 0;
        size_type not_equal_degree_vertices_count = 0;
        for (const vertex_id_type v : vertices()) {
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
        for (const vertex_id_type v : vertices()) {
            if (outbound_degree[v] > 0) {
                root = v;
            }
        }
    }

    std::vector<bool> used_edge(edges_count(), false);
    auto mark_used = [&used_edge, this](const edge_id_type edge_id) {
        used_edge[edge_id] = true;
        if (!is_directed()) {
            used_edge[edge_id ^ 1] = true;  // mark reversed edge as directed
        }
    };

    std::vector<typename EdgesList::const_iterator> current_edge_pointer(vertices_count_);
    for (const vertex_id_type v : vertices()) {
        current_edge_pointer[v] = edges_[v].cbegin();
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
        if (iterator == edges_[v].cend()) {
            result.emplace_back(last_edge_id);
            stack.pop();
            continue;
        }

        const edge_id_type next_edge_id = *iterator++;
        if (used_edge[next_edge_id]) {
            continue;
        }
        mark_used(next_edge_id);
        stack.push({to(next_edge_id), next_edge_id});
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
