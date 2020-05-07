#pragma once
#include <vector>

#include "string_view.hpp"

std::vector<std::size_t> prefix_function(const StringView& string) {
	std::vector<std::size_t> p(string.length(), 0);
	for (std::size_t i = 1; i < string.length(); ++i) {
		std::size_t j = p[i - 1];
		while (j > 0 && string[i] != string[j]) {
			j = p[j - 1];
		}
		if (string[i] == string[j]) {
			++j;
		}
		p[i] = j;
	}
	return p;
}
