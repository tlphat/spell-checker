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
        bloom_filter.InitFromDictionary(in);
        in.close();

        std::ofstream os("words.bf", std::ios::binary);
        bloom_filter.SaveBinary(os);
        os.close();

        return 0;
    }

    BloomFilter bloom_filter;

    std::ifstream in("words.bf", std::ios::binary);
    int load_status_code = bloom_filter.LoadBinary(in);
    in.close();

    if (load_status_code != 0) {
        return 1;
    }

    std::vector<char*> spelt_wrong_words;
    for (int i = 1; i < argc; ++i) {
        if (!bloom_filter.Exists(argv[i])) {
            spelt_wrong_words.push_back(argv[i]);
        }
    }

    if (spelt_wrong_words.empty()) {
        std::cout << "All words are spelt correctly." << std::endl;
        return 0;
    }

    std::cout << "These words are spelt wrong:" << std::endl;
    for (char* word : spelt_wrong_words) {
        std::cout << " " << word << std::endl;
    }
    return 0;
}
