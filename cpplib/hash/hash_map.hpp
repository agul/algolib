#pragma once
#include <cstdlib>

namespace oaht
{

	using std::size_t;

	enum class node_state {
		FREE,
		IN_USE,
		ERASED
	};

	template<typename _Key, typename _Value>
	struct node {
		_Key key;
		_Value value;
		node_state state = node_state::FREE;
	};

	template<typename _Key, typename _Value, typename _Hash = std::hash<_Key>>
	class hash_map {
	public:		
		const node<_Key, _Value> UNDEFINED;

		explicit hash_map(size_t capacity) : capacity(capacity), size(0), UNDEFINED(node<_Key, _Value>()) {
			nodes = new node<_Key, _Value>[capacity];

			for (size_t i = 0; i < capacity; i++)
				nodes[i] = node<_Key, _Value>();
		}

		hash_map() : hash_map(3) {
		}

		size_t count(const _Key& key) const {
			size_t index = get_index(key, capacity);

			for (size_t d = 0; d < capacity; d++) {
				if (nodes[index].state == node_state::FREE)
					return false;
				if (nodes[index].state == node_state::IN_USE
					&& nodes[index].key == key)
					return true;
				index++;
				if (index == capacity)
					index = 0;
			}

			return false;
		}

		const node<_Key, _Value>& find(const _Key& key) const {
			size_t index = get_index(key, capacity);
			for (size_t i = 0; i < capacity; i++) {
				if (nodes[index].key == key) {
					return nodes[index];
				}
				index++;
				if (index == capacity)
					index = 0;
			}
			return UNDEFINED;
		}

		_Value& operator[](const _Key& key) {
			if ((size << 1) > capacity)
				rehash();

			size_t index;
			bool result = put(key, index, nodes, capacity);
			if (result)
				size++;

			return nodes[index].value;
		}

		void clear() {
			for (size_t d = 0; d < capacity; d++) {
				nodes[d].state = node_state::FREE;
			}
			size = 0;
		}

		~hash_map() {
			delete[] nodes;
		}

		size_t capacity;
		size_t size;

		node<_Key, _Value>* nodes;
		
	private:
		size_t get_index(const _Key& key, size_t size) const {
			return (h(key) * 22543) % size;
		}

		void rehash() {
			size_t n_capacity = (capacity << 1);

			node<_Key, _Value>* n_nodes = new node<_Key, _Value>[n_capacity];
			for (size_t i = 0; i < n_capacity; i++)
				n_nodes[i] = node<_Key, _Value>();

			for (size_t i = 0; i < capacity; i++)
			if (nodes[i].state == node_state::IN_USE) {
				size_t index;
				put(nodes[i].key, index, n_nodes, n_capacity);
				n_nodes[index].value = nodes[i].value;
			}

			delete[] nodes;

			nodes = n_nodes;
			capacity = n_capacity;
		}

		bool put(const _Key& key, size_t& index, node<_Key, _Value>* nodes, size_t nodes_length) {
			index = get_index(key, nodes_length);

			for (size_t i = 0; i < nodes_length; i++) {
				if (nodes[index].state == node_state::FREE
					|| nodes[index].state == node_state::ERASED) {
					nodes[index].key = key;
					nodes[index].state = node_state::IN_USE;
					nodes[index].value = 0;
					return true;
				}

				if (nodes[index].key == key)
					return false;

				index++;
				if (index == nodes_length)
					index = 0;
			}

			return false;
			// throw std::logic_error("Unexpected case.");
		}

		_Hash h;
	};

}
