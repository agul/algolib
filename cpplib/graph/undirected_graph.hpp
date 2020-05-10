#pragma once
#include <type_traits>

#include "dsu.hpp"
#include "graph.hpp"

template<typename T = long long, size_t MASK = 0>
class UndirectedGraph : public Graph<T, MASK> {
public:
	UndirectedGraph() : UndirectedGraph(0) {}

	explicit UndirectedGraph(const size_t vertices_count) : Graph<T, MASK>(vertices_count) {}

	template<size_t Mask = MASK, typename std::enable_if<!is_weighted<Mask>::value>::type* = nullptr>
	void add_bidirectional_edge(const size_t from, const size_t to) {
		this->add_directed_edge(from, to);
		this->add_directed_edge(to, from);
	}

	template<size_t Mask = MASK, typename std::enable_if<is_weighted<Mask>::value>::type* = nullptr>
	void add_bidirectional_edge(const size_t from, const size_t to, const T weight) {
		this->add_directed_edge(from, to, weight);
		this->add_directed_edge(to, from, weight);
	}

	template<size_t Mask = MASK, typename std::enable_if<!is_weighted<Mask>::value>::type* = nullptr>
	void add_bidirectional_edge(const typename Graph<T, MASK>::Edge& edge) {
		add_bidirectional_edge(edge.from(), edge.to());
	}

	template<size_t Mask = MASK, typename std::enable_if<is_weighted<Mask>::value>::type* = nullptr>
	void add_bidirectional_edge(const typename Graph<T, MASK>::Edge& edge) {
		add_bidirectional_edge(edge.from(), edge.to(), edge.weight());
	}

	template<size_t Mask = MASK, typename std::enable_if<is_weighted<Mask>::value>::type* = nullptr>
	T minimal_spanning_tree(std::vector<size_t>* mst = nullptr) const {
		std::vector<size_t> graph_edges(this->edges_count());
		std::iota(graph_edges.begin(), graph_edges.end(), 0);
		std::sort(graph_edges.begin(), graph_edges.end(), [&](const size_t& lhs, const size_t& rhs) {
			return this->weight(lhs) < this->weight(rhs);
		});
		DSU dsu(this->vertices_count_);
		T total_weight = 0;
		std::vector<size_t> tree;
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

	bool is_connected() const;
};

template<typename T, size_t MASK>
bool UndirectedGraph<T, MASK>::is_connected() const {
	DSU dsu(this->vertices_count_);
	for (const auto& it : this->edges()) {
		dsu.unite(it.from(), it.to());
	}
	return dsu.sets_count() == 1;
}
