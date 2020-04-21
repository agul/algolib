#pragma once
#include <vector>

#include "graph/undirected_graph.hpp"

class GraphBridges {
public:
	template<typename T, size_t MASK>
	GraphBridges(const UndirectedGraph<T, MASK>& graph) : fup_(graph.vertices_count()),
		tin_(graph.vertices_count()), used_(graph.vertices_count()), timer_(0) {
		find_bridges(graph);
	}

	const std::vector<size_t>& bridges() const {
		return bridges_;
	}

private:
	template<typename T, size_t MASK>
	void find_bridges(const UndirectedGraph<T, MASK>& graph) {
		for (const size_t v : graph.vertices()) {
			if (!used_[v]) {
				dfs(graph, v, 0, true);
			}
		}
	}

	template<typename T, size_t MASK>
	void dfs(const UndirectedGraph<T, MASK>& graph, const size_t vertex, const size_t prev_edge, const bool first) {
		used_[vertex] = true;
		tin_[vertex] = timer_;
		fup_[vertex] = timer_;
		++timer_;
		for (const auto& it : graph.edges(vertex)) {
			if (!first && it.id() == (prev_edge ^ 1)) {
				continue;
			}
			const size_t to = it.to();
			if (used_[to]) {
				umin(fup_[vertex], tin_[to]);
			}
			else {
				dfs(graph, to, it.id(), false);
				umin(fup_[vertex], fup_[to]);
				if (fup_[to] > tin_[vertex]) {
					bridges_.emplace_back(it.id());
				}
			}
		}
	}

	std::vector<size_t> bridges_;
	std::vector<size_t> fup_;
	std::vector<size_t> tin_;
	std::vector<bool> used_;
	size_t timer_;
};
