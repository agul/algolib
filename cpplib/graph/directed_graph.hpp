#pragma once
#include <vector>

#include "graph/graph.hpp"

template<typename T = long long, size_t MASK = 0>
class DirectedGraph : public Graph<T, MASK> {
public:

	bool top_sort_acyclic(std::vector<size_t>& order) const;

};

template<typename T, size_t MASK>
bool DirectedGraph<T, MASK>::top_sort_acyclic(std::vector<size_t>& order) const
// non-recursive topological sorting, works only for acyclic graphs 
{
	order.clear();
	std::vector<size_t> inbound_degree(this->vertex_count_, 0);
	for (const auto& to_vertex : this->to_) {
		++inbound_degree[to_vertex];
	}
	for (const size_t i : this->vertices()) {
		if (inbound_degree[i] == 0) {
			order.emplace_back(i);
		}
	}
	size_t head = 0;
	while (head < order.size()) {
		const size_t cur_vertex = order[head++];
		for (const auto& it : this->edges(cur_vertex)) {
			const size_t to_vertex = it.to();
			--inbound_degree[to_vertex];
			if (inbound_degree[to_vertex] == 0) {
				order.emplace_back(to_vertex);
			}
		}
	}
	return order.size() == this->vertex_count_;
}
