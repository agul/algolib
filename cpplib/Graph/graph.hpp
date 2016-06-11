#pragma once
#include "Head.h"
#include "maths.hpp"

enum GraphType {
	Weighted = 1,
	Flow = 2
};

template<typename T = ll, size_t MASK = 0>
class Graph {
public:

	Graph() = default;

	virtual ~Graph() {
		clear();
	}

	void init(const size_t vertex_count) {
		clear();
		vertex_count_ = vertex_count;
		edges_.resize(vertex_count_);
	}

	void clear();

	template<const size_t Mask = MASK, typename std::enable_if<(Mask & GraphType::Weighted) == 0>::type* = nullptr>
	void add_directed_edge(const size_t from, const size_t to) {
		push_edge(from, to);
	}

	template<const size_t Mask = MASK, typename std::enable_if<(Mask & GraphType::Weighted) != 0>::type* = nullptr>
	void add_directed_edge(const size_t from, const size_t to, const T weight) {
		weight_.emplace_back(weight);
		push_edge(from, to);
	}

	bool is_sparse() const {
		return vertex_count_ == 0 || sqr(static_cast<ll>(vertex_count_)) >= (edges_count_ << 4);
	}

	size_t find_vertex_with_max_degree() const;

protected:

	void push_edge(const size_t from, const size_t to) {
		const size_t edge_id = from_.size();
		from_.emplace_back(from);
		to_.emplace_back(to);
		edges_[from].emplace_back(edge_id);
	}

public:
	std::vector<std::vector<size_t>> edges_;
	std::vector<size_t> from_;
	std::vector<size_t> to_;
	std::vector<T> weight_;

	size_t vertex_count_;

};

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

template<typename T, size_t MASK>
void Graph<T, MASK>::clear() {
	vertex_count_ = 0;
	edges_.clear();
	from_.clear();
	to_.clear();
	weight_.clear();
}

template<typename T, size_t MASK>
size_t Graph<T, MASK>::find_vertex_with_max_degree() const {
	const auto iter = std::max_element(edges_.begin(), edges_.end(), [](const std::vector<size_t>& lhs, const std::vector<size_t>& rhs) {
		return lhs.size() < rhs.size();
	});
	return static_cast<size_t>(std::distance(edges_.begin(), iter));
}

/* template<typenamå T, size_t MASK>
bool Graph::top_sort_acyclic(std::vector<size_t>& order) const
// non-recursive topological sorting, works only for acyclic graphs 
{
	order.clear();
	std::vector<size_t> inbound_degree(vertex_count_, 0);
	for (const auto& to_vertex : to_) {
		++inbound_degree[to_vertex];
	}
	for (size_t i = 0; i < vertex_count_; ++i) {
		if (inbound_degree[i] == 0) {
			order.emplace_back(i);
		}
	}
	size_t head = 0;
	while (head < order.size()) {
		const size_t cur_vertex = order[head++];
		for (const auto& it : edges[cur_vertex]) {
			const size_t to_vertex = to_[it];
			--inbound_degree[to_vertex];
			if (inbound_degree[to_vertex] == 0) {
				order.emplace_back(to_vertex);
			}
		}
	}
	return order.size() == vertex_count_;
} */
