#pragma once
#include <vector>

#include "graph.hpp"

template<typename T = int64_t, size_t MASK = 0>
class DirectedGraph : public Graph<T, MASK> {
public:
	DirectedGraph() : DirectedGraph(0) {}

	explicit DirectedGraph(const size_t vertices_count) : Graph<T, MASK>(vertices_count) {}

	bool is_acyclic() const;
	bool top_sort_acyclic(std::vector<size_t>* vertex_order = nullptr) const;
	void top_sort_rec(std::vector<size_t>* vertex_order) const;

	bool solve_2_sat(std::vector<size_t>* component_id = nullptr) const;

	DirectedGraph reversed() const;
	
	size_t scc(std::vector<size_t>* vertex_color = nullptr) const;

private:
	void top_sort_rec_impl(const size_t vertex, std::vector<size_t>& order, std::vector<bool>& used) const;
};

template<typename T, size_t MASK>
DirectedGraph<T, MASK> DirectedGraph<T, MASK>::reversed() const {
	DirectedGraph<T, MASK> result(this->vertices_count_);
	for (const auto& edge : this->edges()) {
		result.add_directed_edge(edge.reversed());
	}
	return result;
}

template<typename T, size_t MASK>
bool DirectedGraph<T, MASK>::is_acyclic() const {
	return this->top_sort_acyclic();
}

template<typename T, size_t MASK>
bool DirectedGraph<T, MASK>::solve_2_sat(std::vector<size_t>* component_id) const
// requires graph with 2N vertices:
// each initial vertex should be duplicated as v -> (v * 2, v * 2 + 1) as (v, !v)
// returns false if the 2-SAT problem has no solution
{
    std::vector<size_t> order;
    this->top_sort_rec(&order);

    std::vector<size_t> component(this->vertices_count_, std::numeric_limits<size_t>::max());
    const auto reversed = this->reversed();
    std::function<void(size_t, size_t)> dfs = [&reversed, &dfs, &component](const size_t v, const size_t component_id) {
        component[v] = component_id;
        for (const auto& edge : reversed.edges(v)) {
            const size_t to = edge.to();
            if (component[to] == std::numeric_limits<size_t>::max()) {
                dfs(to, component_id);
            }
        }
    };

    size_t components_count = 0;
    for (const size_t v : order) {
        if (component[v] == std::numeric_limits<size_t>::max()) {
            dfs(v, components_count++);
        }
    }

    bool result = true;
    for (const size_t v : this->vertices()) {
        if (component[v] == component[v ^ 1]) {
            result = false;
            break;
        }
    }
    if (component_id != nullptr) {
        component_id->swap(component);
    }
    return result;
}

template<typename T, size_t MASK>
bool DirectedGraph<T, MASK>::top_sort_acyclic(std::vector<size_t>* vertex_order) const
// non-recursive topological sorting, works only for acyclic graphs 
{
	std::vector<size_t> order;
	std::vector<size_t> inbound_degree(this->vertices_count_, 0);
	for (const auto& to : this->to_) {
		++inbound_degree[to];
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
			const size_t to = it.to();
			--inbound_degree[to];
			if (inbound_degree[to] == 0) {
				order.emplace_back(to);
			}
		}
	}
	const bool result = (order.size() == this->vertices_count_);
	if (vertex_order != nullptr) {
		vertex_order->swap(order);
	}
	return result;
}

template<typename T, size_t MASK>
void DirectedGraph<T, MASK>::top_sort_rec(std::vector<size_t>* vertex_order) const {
	std::vector<bool> used(this->vertices_count_, false);
	std::vector<size_t> order;
	for (const size_t v : this->vertices()) {
		if (!used[v]) {
			top_sort_rec_impl(v, order, used);
		}
	}
	std::reverse(order.begin(), order.end());
	vertex_order->swap(order);
}

template<typename T, size_t MASK>
void DirectedGraph<T, MASK>::top_sort_rec_impl(const size_t vertex, std::vector<size_t>& order, std::vector<bool>& used) const {
	used[vertex] = true;
	for (const auto& it : this->edges(vertex)) {
		const size_t to = it.to();
		if (!used[to]) {
			top_sort_rec_impl(to, order, used);
		}
	}
	order.emplace_back(vertex);
}

template<typename T, size_t MASK>
size_t DirectedGraph<T, MASK>::scc(std::vector<size_t>* vertex_color) const {
	const DirectedGraph<T, MASK> rev_graph = reversed();
	std::vector<size_t> order(this->vertices_count_);
	top_sort_rec(&order);
	std::vector<bool> used(this->vertices_count_, false);
	std::vector<size_t> color(this->vertices_count_);
	Queue<size_t> queue(this->vertices_count_);
	size_t components_count = 0;
	for (const auto& v : order) {
		if (!used[v]) {
			queue.clear();
			queue.push(v);
			while (!queue.empty()) {
				const size_t vertex = queue.pop_front();
				color[vertex] = components_count;
				used[vertex] = true;
				for (const auto& it : rev_graph.edges(vertex)) {
					const size_t to = it.to();
					if (!used[to]) {
						used[to] = true;
						queue.push(to);
					}
				}
			}
			++components_count;
		}
	}
	if (vertex_color != nullptr) {
		vertex_color->swap(color);
	}
	return components_count;
}
