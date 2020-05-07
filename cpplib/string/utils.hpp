#pragma once
#include <algorithm>
#include <cctype>
#include <string>

inline bool is_lower(const char ch) {
	return std::islower(ch);
}

inline bool is_upper(const char ch) {
	return std::isupper(ch);
}

inline bool is_alpha(const char ch) {
	return std::isalpha(ch);
}

inline bool is_digit(const char ch) {
	return std::isdigit(ch);
}

inline char to_upper(const char ch) {
	return is_upper(ch) ? ch : ch ^ 32;
}

inline char to_lower(const char ch) {
	return is_lower(ch) ? ch : ch ^ 32;
}

inline std::string to_upper(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(), to_upper);
	return s;
}

inline std::string to_lower(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(), to_lower);
	return s;
}

inline uint8_t to_digit(const char ch)
// get exact numeric value of the corresponding char
// REQUIRE: char from '0' to '9'
{
	return ch - '0';
}

