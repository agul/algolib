#pragma once
#include <cstring>
#include <string>

class StringView {
public:
	using iterator = const char*;

	/// caide keep
	constexpr StringView() : data_(nullptr), length_(0) {}

	constexpr StringView(const StringView&) = default;
	constexpr StringView(StringView&&) = default;
	StringView& operator =(const StringView&) = default;
	StringView& operator =(StringView&&) = default;

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

	constexpr size_t size() const {
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

	StringView substr(const size_t pos) const {
		return StringView(data_ + pos, length_ - pos);
	}

	StringView substr(const size_t pos, const size_t len) const {
		return StringView(data_ + pos, len);
	}

	size_t find(const char ch, const size_t pos) const {
		for (size_t i = pos; i < length_; ++i) {
			if (data_[i] == ch) {
				return i;
			}
		}
		return std::string::npos;
	}

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

bool operator ==(const StringView& lhs, const StringView& rhs) {
	if (lhs.length_ != rhs.length_) {
		return false;
	}
	for (size_t i = 0; i < lhs.length_; ++i) {
		if (lhs.data_[i] != rhs.data_[i]) {
			return false;
		}
	}
	return true;
}

bool operator ==(const StringView& lhs, const char* rhs) {
	for (size_t i = 0; i < lhs.length_; ++i) {
		if (rhs[i] == 0 || rhs[i] != lhs.data_[i]) {
			return false;
		}
	}
	return rhs[lhs.length_] == 0;
}

bool operator ==(const char* lhs, const StringView& rhs) {
	return rhs == lhs;
}

bool operator !=(const StringView& lhs, const StringView& rhs) {
	return !(lhs == rhs);
}

bool operator !=(const StringView& lhs, const char* rhs) {
	return !(lhs == rhs);
}

bool operator !=(const char* lhs, const StringView& rhs) {
	return !(rhs == lhs);
}
