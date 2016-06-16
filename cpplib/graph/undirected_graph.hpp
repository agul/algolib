#pragma once
#include <type_traits>

#include "graph/graph.hpp"

template<typename T = long long, size_t MASK = 0>
class UndirectedGraph : public Graph<T, MASK> {
public:
	UndirectedGraph() = default;

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

};
