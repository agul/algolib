#pragma once
#include "Head.h"
#include "maths.hpp"
#include "range/ranges.hpp"

enum GraphType {
	Weighted = 1
};

template<typename T = ll, size_t MASK = 0>
class Graph {
public:

	using EdgesList = std::vector<size_t>;

	class Edge {
	public:

		explicit Edge(const std::vector<size_t>& from, const std::vector<size_t>& to, const std::vector<T>& weight, const size_t index) :
			from_(from), to_(to), weight_(weight), index_(index) {}

		size_t from() const {
			return from_[index_];
		}

		size_t to() const {
			return to_[index_];
		}

		template<const size_t Mask = MASK, typename std::enable_if<(Mask & GraphType::Weighted) != 0>::type* = nullptr>
		size_t weight() const {
			return weight_[index_];
		}

		size_t id() const {
			return index_;
		}

		void set_index(const size_t index) {
			index_ = index;
		}

	private:
		const std::vector<size_t>& from_;
		const std::vector<size_t>& to_;
		const std::vector<T>& weight_;
		size_t index_;

	};

	class EdgesListHolder {
	public:

		using BaseConstIterator = EdgesList::const_iterator;

		class ConstIterator : public BaseConstIterator {
		public:
			using value_type = Edge;
			using pointer = const value_type*;
			using const_reference = const value_type&;

			explicit ConstIterator(BaseConstIterator it, const std::vector<size_t>& from, const std::vector<size_t>& to, const std::vector<T>& weight) :
				BaseConstIterator(it), cur_edge_(from, to, weight, 0) {}

			pointer operator->() {
				return addressof(operator*());
			}

			const_reference operator*() {
				const BaseConstIterator::value_type index = this->BaseConstIterator::operator*();
				cur_edge_.set_index(index);
				return cur_edge_;
			}

		private:
			Edge cur_edge_;

		};

		using const_iterator = ConstIterator;
		using value_type = typename ConstIterator::value_type;

		explicit EdgesListHolder(const EdgesList& edges, const std::vector<size_t>& from, const std::vector<size_t>& to, const std::vector<T>& weight) :
			edges_(edges), from_(from), to_(to), weight_(weight) {}

		const_iterator begin() const {
			return ConstIterator(edges_.begin(), from_, to_, weight_);
		}

		const_iterator end() const {
			return ConstIterator(edges_.end(), from_, to_, weight_);
		}

	private:
		const EdgesList& edges_;
		const std::vector<size_t>& from_;
		const std::vector<size_t>& to_;
		const std::vector<T>& weight_;

	};

	Graph() = default;

	virtual ~Graph() {
		clear();
	}

	void init(const size_t vertex_count) {
		clear();
		vertex_count_ = vertex_count;
		edges_.resize(vertex_count_);
	}

	IntegerRange<size_t> vertices() const {
		return range(vertex_count_);
	}

	IntegerIterator<size_t> begin() const {
		return vertices().begin();
	}

	IntegerIterator<size_t> end() const {
		return vertices().end();
	}

	void clear();

	size_t from(const size_t index) const {
		return from_[index];
	}

	size_t to(const size_t index) const {
		return to_[index];
	}

	template<const size_t Mask = MASK, typename std::enable_if<(Mask & GraphType::Weighted) != 0>::type* = nullptr>
	T weight(const size_t index) const {
		return weight_[index];
	}

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

	EdgesListHolder edges(const size_t vertex) const {
		return EdgesListHolder(edges_[vertex], from_, to_, weight_);
	}

	size_t find_vertex_with_max_degree() const;

protected:

	void push_edge(const size_t from, const size_t to) {
		const size_t edge_id = from_.size();
		from_.emplace_back(from);
		to_.emplace_back(to);
		edges_[from].emplace_back(edge_id);
	}

	std::vector<EdgesList> edges_;
	std::vector<size_t> from_;
	std::vector<size_t> to_;
	std::vector<T> weight_;

	size_t vertex_count_;

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
	const auto iter = std::max_element(edges_.begin(), edges_.end(),
		[](const EdgesList& lhs, const EdgesList& rhs) {
		return lhs.size() < rhs.size();
	});
	return static_cast<size_t>(std::distance(edges_.begin(), iter));
}