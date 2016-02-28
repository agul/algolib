#include "StringView.h"

StringView StringView::substr(const size_t pos) const {
	return StringView(data_ + pos, length_ - pos);
}

StringView StringView::substr(const size_t pos, const size_t len) const {
	return StringView(data_ + pos, len);
}

size_t StringView::find(const char ch, const size_t pos) const {
	for (size_t i = pos; i < length_; ++i) {
		if (data_[i] == ch) {
			return i;
		}
	}
	return std::string::npos;
}

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
