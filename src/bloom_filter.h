#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <string>
#include <bitset>
#include <array>
#include <functional>
#include <fstream>

class BloomFilter {
public:
    BloomFilter();

    bool Exists(const std::string& word);

    void LoadDictionary(std::ifstream& is);

    void SaveBinary(std::ofstream& os);
    void LoadBinary(std::ifstream& is);

private:
    void Add(const std::string& word);

    static const size_t kM = 4600000;
    using BitArray = std::bitset<kM>;
    BitArray bit_array_;

    static const size_t kK = 17;
    static const std::array<unsigned int, kK> kKnvOffsets;
    static const std::array<unsigned int, kK> kKnvPrimes;
    using HashFunction = std::function<int(const std::string&)>;
    std::array<HashFunction, kK> hash_functions_;

    static const int kVersion = 1;
};

#endif
