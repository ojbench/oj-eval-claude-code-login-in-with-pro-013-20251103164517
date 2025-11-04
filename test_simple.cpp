#include "src/map.hpp"
#include <iostream>
#include <string>

class Compare {
public:
	bool operator () (const int &lhs, const int &rhs) const {
		return lhs < rhs;
	}
};

int main() {
    sjtu::map<int, std::string, Compare> map;

    // Test insert
    map[1] = "one";
    map[2] = "two";
    map[3] = "three";

    std::cout << "Size: " << map.size() << std::endl;

    // Test iteration
    std::cout << "Forward iteration:" << std::endl;
    for (auto it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    // Test backward iteration
    std::cout << "Backward iteration:" << std::endl;
    auto it = map.end();
    while (it != map.begin()) {
        --it;
        std::cout << it->first << ": " << it->second << std::endl;
    }

    return 0;
}
