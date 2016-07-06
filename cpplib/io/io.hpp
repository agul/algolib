#pragma once
#include <iostream>
#include <utility>
#include <vector>

namespace std {

template<typename T>
std::istream& operator>>(std::istream& in, std::vector<T>& vec) {
	for (auto& it : vec) {
		in >> it;
	}
	return in;
}


template<typename T, typename U>
std::istream& operator>>(std::istream& in, std::pair<T, U>& rhs) {
	in >> rhs.first >> rhs.second;
	return in;
}

void assign_files_input_txt() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
}

}
