#include "bloom_filter.h"

#include <iostream>
#include <fstream>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Too few arguments." << std::endl;
        return 1;
    }

    if (std::strcmp("-build", argv[1]) == 0) {
        if (argc != 3) {
            std::cerr << "Incorrect number of arguments, expecting 1 parameter for build file." << std::endl;
            return 1;
        }

        BloomFilter bloom_filter;

        std::ifstream in(argv[2]);
        bloom_filter.LoadDictionary(in);
        in.close();

        std::ofstream os("words.bf", std::ios::binary);
        bloom_filter.SaveBinary(os);
        os.close();

        return 0;
    }

    std::cerr << "Unsupported operation." << std::endl;

    return 1;
}
