#pragma once

static constexpr uint32_t kDayMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

template<typename T>
inline bool is_leap_year(const T year) {
    return year % 400 == 0 || (year % 100 != 0 && (year & 3) == 0);
}

inline size_t get_days_in_month(const size_t month, const size_t year) {
    return kDayMonth[month] + (month == 1 && is_leap_year(year) ? 1 : 0);
}
