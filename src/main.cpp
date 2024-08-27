#include "bloom_filter.h"

#include <iostream>
#include <fstream>

int main() {
    BloomFilter bloom_filter;

    std::ifstream in("dict.txt");
    bloom_filter.LoadDictionary(in);
    in.close();

    std::ofstream os("words.bf", std::ios::binary);
    bloom_filter.SaveBinary(os);
    os.close();

    std::cout << bloom_filter.Exists("word") << std::endl;

    return 0;
}
