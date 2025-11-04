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

    // Test operator[] with empty strings (like the actual test)
    for (int i = 0; i < 10; ++i) {
        std::cout << map[i];
    }
    std::cout << " size=" << map.size() << std::endl;

    // Test backward iteration after multiple inserts
    std::cout << "Backward iteration:" << std::endl;
    auto it = map.end();
    while (it != map.begin()) {
        --it;
        std::cout << it->first << " ";
    }
    std::cout << std::endl;

    // Test erase all
    std::cout << "Erasing all elements" << std::endl;
    while (map.begin() != map.end()) {
        map.erase(map.begin());
    }
    std::cout << "Size after erase: " << map.size() << std::endl;

    return 0;
}
