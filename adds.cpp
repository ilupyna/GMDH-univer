#include "adds.h"


void print(const std::vector<std::vector<double>>& v) {
	for(auto ryad:v) {
		for(auto el:ryad) {
			std::cout<< el <<"\t";
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;
}
