#pragma once
#include "graph/graph.hpp"

template<typename T = ll, size_t MASK = 0>
class UndirectedGraph : public Graph<T, MASK> {
public:

	UndirectedGraph() = default;

	template<const size_t Mask = MASK, typename std::enable_if<(Mask & GraphType::Weighted) == 0>::type* = nullptr>
	void add_bidirectional_edge(const size_t from, const size_t to) {
		this->add_directed_edge(from, to);
		this->add_directed_edge(to, from);
	}

	template<const size_t Mask = MASK, typename std::enable_if<(Mask & GraphType::Weighted) != 0>::type* = nullptr>
	void add_bidirectional_edge(const size_t from, const size_t to, const T weight) {
		this->add_directed_edge(from, to, weight);
		this->add_directed_edge(to, from, weight);
	}

};
