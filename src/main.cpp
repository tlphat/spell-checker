#include "bloom_filter.h"

#include <iostream>
#include <fstream>

int main() {
    BloomFilter bloom_filter;
    std::ifstream in("dict.txt");
    std::string line;
    while (std::getline(in, line)) {
        bloom_filter.add(line);
    }
    in.close();

    std::cout << bloom_filter.exists("word") << std::endl;

    return 0;
}
