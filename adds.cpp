#include <iostream>
#include <vector>

template <class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v) {
	os << "{";
	bool first = true;
	for (const auto& x : v) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "}";
}

void print(const std::vector<std::vector<double>>& v) {
	for(auto ryad:v) {
		for(auto el:ryad) {
			std::cout<< el <<"\t";
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;
}
