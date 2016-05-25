#pragma once
#include "Head.h"

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

	/* bool operator ==(const Queue& rhs) const {
	return top_index_ == rhs.top_index_ && data_ == rhs.data_;
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
	} */

private:
	container data_;
	size_type head_index_;
	size_type tail_index_;

};
