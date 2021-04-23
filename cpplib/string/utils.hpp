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

template<typename T>
inline T to_upper(T);

template<typename T>
inline T to_lower(T);

template<>
inline char to_upper<char>(const char ch) {
    return is_lower(ch) ? ch ^ 32 : ch;
}

template<>
inline char to_lower<char>(const char ch) {
    return is_upper(ch) ? ch ^ 32 : ch;
}

template<>
inline std::string to_upper<std::string>(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), to_upper<char>);
    return s;
}

template<>
inline std::string to_lower<std::string>(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), to_lower<char>);
    return s;
}

inline uint8_t to_digit(const char ch)
// get exact numeric value of the corresponding char
// REQUIRE: char from '0' to '9'
{
    return ch - '0';
}

bool starts_with(const std::string& s, const std::string& t) {
    return s.length() >= t.length() && s.compare(0, t.length(), t) == 0;
}

bool starts_with(const std::string& s, const char ch) {
    return !s.empty() && s.front() == ch;
}

bool ends_with(const std::string& s, const std::string& t) {
    return s.length() >= t.length() && s.compare(s.length() - t.length(), t.length(), t) == 0;
}

bool ends_with(const std::string& s, const char ch) {
    return !s.empty() && s.back() == ch;
}
