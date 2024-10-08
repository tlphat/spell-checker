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

    void InitFromDictionary(std::ifstream& is);

    void SaveBinary(std::ofstream& os);
    int LoadBinary(std::ifstream& is);

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

    static void WriteStrToBinFile(std::ofstream& os, const std::string& str, int size_in_bytes);
    static void WriteIntToBinFile(std::ostream& os, int n, int size_in_bytes);
    void WriteBitArrToBinFile(std::ostream& os);

    static bool AreNextNBytesFromFileMatchStr(std::istream& is, const std::string& str, int n);
    static bool AreNextNBytesFromFileMatchInt(std::istream& is, int target, int n);
    void LoadBitArrFromBinFile(std::istream& is);
};

#endif
