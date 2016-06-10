#pragma once
#include <functional>
#include <vector>

template<typename T>
class Stack {
public:
	using value_type = T;
	using size_type = size_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using rvalue = value_type&&;
	using container = std::vector<value_type>;

	Stack(const size_type max_size) : data_(container(max_size)) {
		clear();
	}

	Stack() = delete;

	Stack(const Stack&) = default;
	Stack& operator =(const Stack&) = default;
	Stack(Stack&&) = default;
	Stack& operator =(Stack&&) = default;

	reference top() {
		return data_[top_index_ - 1];
	}

	const_reference top() const {
		return data_[top_index_ - 1];
	}

	constexpr bool empty() const {
		return top_index_ == 0;
	}

	void clear() {
		top_index_ = 0;
	}

	constexpr size_type size() const {
		return top_index_;
	}

	void push(const_reference value) {
		data_[top_index_++] = value;
	}

	void push(rvalue value) {
		data_[top_index_++] = std::move(value);
	}

	value_type pop() {
		return data_[--top_index_];
	}

	void swap(Stack& rhs) {
		data_.swap(rhs.data_);
		std::swap(top_index_, rhs.top_index_);
	}

	bool operator ==(const Stack& rhs) const {
		return top_index_ == rhs.top_index_ && data_ == rhs.data_;
	}

	bool operator !=(const Stack& rhs) const {
		return !operator==(rhs);
	}

	bool operator <(const Stack& rhs) const {
		return data_ < rhs.data_;
	}

	bool operator <=(const Stack& rhs) const {
		return data_ <= rhs.data_;
	}

	bool operator >(const Stack& rhs) const {
		return data_ > rhs.data_;
	}

	bool operator >=(const Stack& rhs) const {
		return data_ >= rhs.data_;
	}

private:
	container data_;
	size_type top_index_;

};

template<typename T>
class CmpStack {
public:
	using value_type = T;
	using size_type = size_t;
	using const_reference = const value_type&;
	using Comparator = std::function<bool(const_reference, const_reference)>;

	CmpStack(const size_type n, const Comparator& less = std::less<value_type>()) : stack_(n), less_(less) {}

	constexpr bool empty() const {
		return stack_.empty();
	}

	value_type min_value() const {
		return stack_.empty() ? 0 : stack_.top().second;
	}

	void push(const_reference element) {
		value_type cur_min = element;
		if (!stack_.empty() && less_(stack_.top().second, cur_min)) {
			cur_min = stack_.top().second;
		}
		stack_.push(std::make_pair(element, cur_min));
	}

	value_type pop() {
		return stack_.pop().first;
	}

	void clear() {
		stack_.clear();
	}

private:
	Stack<std::pair<value_type, value_type>> stack_;
	Comparator less_;

};
