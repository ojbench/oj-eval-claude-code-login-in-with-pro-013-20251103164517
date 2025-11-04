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
        map[i];
    }
    std::cout << "Size: " << map.size() << std::endl;

    // Test erase all
    std::cout << "Erasing all elements one by one" << std::endl;
    int count = 0;
    while (map.begin() != map.end()) {
        std::cout << "Erasing element " << count << ", size before: " << map.size() << std::endl;
        try {
            map.erase(map.begin());
            std::cout << "  Erased successfully, size after: " << map.size() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "  Exception: " << e.what() << std::endl;
            break;
        }
        count++;
    }
    std::cout << "Final size: " << map.size() << std::endl;

    return 0;
}
