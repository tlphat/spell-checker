#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <string>
#include <bitset>
#include <array>
#include <functional>

class BloomFilter {
public:
    BloomFilter();

    void add(const std::string& word);
    bool exists(const std::string& word);

private:
    static const size_t M = 4600000;
    using BitArray = std::bitset<M>;
    BitArray bit_array;

    static const size_t K = 17;
    static const std::array<unsigned int, K> knv_offsets;
    static const std::array<unsigned int, K> knv_primes;
    using HashFunction = std::function<int(const std::string&)>;
    std::array<HashFunction, K> hash_functions;
};

#endif
