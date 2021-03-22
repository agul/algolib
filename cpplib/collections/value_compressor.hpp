#pragma once
#include <vector>

#include "hash/safe_integral_hash.hpp"

template<typename T, typename HashFunction = std::hash<T>>
class ValueCompressor : public std::vector<T> {
public:
	using size_type = std::size_t;
	using value_type = T;
	using base_container_type = std::vector<value_type>;

	using hash_function_type = HashFunction;
	using hash_map_type = std::unordered_map<value_type, size_type, hash_function_type>;

	ValueCompressor() : base_container_type() {}

	template<typename C>
	explicit ValueCompressor(const C& collection) : ValueCompressor(collection.begin(), collection.end()) {}

	template<typename I>
	ValueCompressor(const I first, const I last) : base_container_type() {
		for (I it = first; it != last; ++it) {
			add(*it);
		}
	}

	const value_type& get_by_id(const size_type index) const {
		return this->at(index);
	}

	size_type add(const value_type& value) {
		return add_impl(value);
	}

	size_type operator [](const value_type& value) {
		return add_impl(value);
	}

	bool contains(const value_type& value) const {
		return ids_.find(value) != ids_.cend();
	}

	hash_map_type& ids() {
		return ids_;
	}

	const hash_map_type& ids() const {
		return ids_;
	}

	void swap(ValueCompressor& rhs) {
		base_container_type::swap(rhs);
		ids_.swap(rhs.ids_);
	}

private:
	hash_map_type ids_;

	size_type add_impl(const value_type& value) {
		const auto it = ids_.find(value);
		if (it != ids_.cend()) {
			return it->second;
		}

		const size_t id = this->size();
		ids_[value] = id;
		this->emplace_back(value);
		return id;
	}
};

template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
using IntegralValueCompressor = ValueCompressor<T, SafeIntegralHash<T>>;


namespace {

using size_type = std::size_t;

template<typename T, typename Collection, typename ValueCompressorType>
std::vector<size_type> compressed_impl(const Collection& collection) {
	std::vector<size_type> result;
	result.reserve(collection.size());
	ValueCompressorType compressor;
	for (const auto& it : collection) {
		result.emplace_back(compressor.add(it));
	}
	return result;
}

}  // namespace


template<typename T, typename Collection, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
std::vector<std::size_t> compressed(const Collection& collection) {
	return compressed_impl<T, Collection, IntegralValueCompressor<T>>(collection);
}

template<typename T, typename Collection, typename std::enable_if<!std::is_integral<T>::value>::type* = nullptr>
std::vector<std::size_t> compressed(const Collection& collection) {
	return compressed_impl<T, Collection, ValueCompressor<T>>(collection);
}
