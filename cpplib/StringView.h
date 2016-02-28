#pragma once
#include "Head.h"

class StringView {
public:
	using iterator = const char*;

	constexpr StringView() : data_(nullptr), length_(0) {}
	constexpr StringView(const StringView&) = default;

	StringView(const char* begin, const char* end) : data_(begin), length_(static_cast<size_t>(end - begin)) {}
	StringView(const char* data, size_t length) : data_(data), length_(length) {}
	StringView(const char* data) : data_(data), length_(strlen(data)) {}
	StringView(const std::string& data) : data_(data.c_str()), length_(data.length()) {}

	constexpr char operator[](const size_t index) const {
		return data_[index];
	}

	constexpr const char* data() const {
		return data_;
	}

	constexpr const char* c_str() const {
		return data_;
	}

	constexpr size_t length() const {
		return length_;
	}

	constexpr bool empty() const {
		return length_ == 0;
	}

	constexpr char front() const {
		return data_[0];
	}

	constexpr char back() const {
		return data_[length_ - 1];
	}

	std::string to_string() const {
		return{ begin(), end() };
	}

	constexpr iterator begin() const {
		return data_;
	}

	constexpr iterator end() const {
		return data_ + length_;
	}

	StringView substr(const size_t pos) const;
	StringView substr(const size_t pos, const size_t len) const;
	size_t find(const char ch, const size_t pos = 0) const;

	std::string operator +(const StringView& rhs) const;

	friend bool operator ==(const StringView& lhs, const StringView& rhs);
	friend bool operator ==(const StringView& lhs, const char* rhs);
	friend bool operator ==(const char* lhs, const StringView& rhs);
	friend bool operator !=(const StringView& lhs, const StringView& rhs);
	friend bool operator !=(const StringView& lhs, const char* rhs);
	friend bool operator !=(const char* lhs, const StringView& rhs);

private:
	const char* data_;
	size_t length_;
};

