#pragma once
#include "stack.hpp"

#include <functional>
#include <vector>

template<typename T>
class Queue {
public:
	using value_type = T;
	using size_type = size_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using rvalue = value_type&&;
	using container = std::vector<value_type>;

	Queue(const size_type max_size) : data_(container(max_size)) {
		clear();
	}

	Queue() = delete;

	Queue(const Queue&) = default;
	Queue& operator =(const Queue&) = default;
	Queue(Queue&&) = default;
	Queue& operator =(Queue&&) = default;

	reference front() {
		return data_[head_index_];
	}

	const_reference front() const {
		return data_[head_index_];
	}

	reference back() {
		return data_[tail_index_ - 1];
	}

	const_reference back() const {
		return data_[tail_index_ - 1];
	}

	constexpr bool empty() const {
		return head_index_ == tail_index_;
	}

	void clear() {
		head_index_ = 0;
		tail_index_ = 0;
	}

	constexpr size_type size() const {
		return tail_index_ - head_index_;
	}

	void push(const_reference value) {
		data_[tail_index_++] = value;
	}

	void push(rvalue value) {
		data_[tail_index_++] = std::move(value);
	}

	value_type pop_front() {
		return data_[head_index_++];
	}

	value_type pop_back() {
		return data_[--tail_index_];
	}

	void swap(Queue& rhs) {
		data_.swap(rhs.data_);
		std::swap(head_index_, rhs.head_index_);
		std::swap(tail_index_, rhs.tail_index_);
	}

	bool operator ==(const Queue& rhs) const {
		return head_index_ == rhs.head_index_ && tail_index_ == rhs.tail_index_ && data_ == rhs.data_;
	}

	bool operator !=(const Queue& rhs) const {
		return !operator==(rhs);
	}

	bool operator <(const Queue& rhs) const {
		return data_ < rhs.data_;
	}

	bool operator <=(const Queue& rhs) const {
		return data_ <= rhs.data_;
	}

	bool operator >(const Queue& rhs) const {
		return data_ > rhs.data_;
	}

	bool operator >=(const Queue& rhs) const {
		return data_ >= rhs.data_;
	}

private:
	container data_;
	size_type head_index_;
	size_type tail_index_;

};

template<typename T>
class CmpQueue {
public:
	using value_type = T;
	using size_type = size_t;
	using const_reference = const value_type&;
	using Comparator = std::function<bool(const_reference, const_reference)>;

	CmpQueue(const size_type n, const Comparator& cmp = std::less<value_type>()) : a_(n, cmp), b_(n, cmp), cmp_(cmp) {}

	constexpr bool empty() const {
		return a_.empty() && b_.empty();
	}

	value_type min_value() const {
		if (a_.empty()) {
			return b_.min_value();
		}
		if (b_.empty()) {
			return a_.min_value();
		}
		const value_type a = a_.min_value();
		const value_type b = b_.min_value();
		return cmp_(a, b) ? a : b;
	}

	void push(const_reference element) {
		a_.push(element);
	}

	value_type pop() {
		if (b_.empty()) {
			while (!a_.empty()) {
				b_.push(a_.pop());
			}
		}
		return b_.pop();
	}

	void clear() {
		a_.clear();
		b_.clear();
	}

private:
	CmpStack<value_type> a_;
	CmpStack<value_type> b_;
	Comparator cmp_;
};
