#include "bloom_filter.h"
#include "hash_utilities.h"

#include <cstring>
#include <algorithm>
#include <bit>

const size_t BloomFilter::kK;
const size_t BloomFilter::kM;
const int BloomFilter::kVersion;

const std::array<unsigned int, BloomFilter::kK> BloomFilter::kKnvOffsets = {
    2166136261, 
    1246123925, 
    126854453, 
    3521477421, 
    1247224771, 
    648291761, 
    342480847, 
    1000293871, 
    2245623112, 
    228154015, 
    1125668724, 
    3057374935, 
    2893741246, 
    3525322568, 
    3523513479, 
    3167490967, 
    769889561,
};

const std::array<unsigned int, BloomFilter::kK> BloomFilter::kKnvPrimes = {
    16777619, 
    135019, 
    821741, 
    236077, 
    617411, 
    266051, 
    575747, 
    784727, 
    120619, 
    454709, 
    587473, 
    10631, 
    689891, 
    429677, 
    958687, 
    607147, 
    584303,
};

BloomFilter::BloomFilter() {
    for (int i = 0; i < kK; ++i) {
        hash_functions_[i] = [i](const std::string& s) -> int { 
            return hash::base_fnv1(s, kKnvOffsets[i], kKnvPrimes[i], kM); 
        };
    }
}

void BloomFilter::Add(const std::string& word) {
    for (HashFunction hash_function : hash_functions_) {
        int i = hash_function(word);
        bit_array_[i] = 1;
    }
}

bool BloomFilter::Exists(const std::string& word) {
    for (HashFunction hash_function : hash_functions_) {
        int i = hash_function(word);
        if (bit_array_[i] == 0) {
            return false;
        }
    }
    return true;
}

void BloomFilter::InitFromDictionary(std::ifstream& is) {
    std::string line;
    while (std::getline(is, line)) {
        Add(line);
    }
}

void BloomFilter::SaveBinary(std::ofstream& os) {
    WriteStrToBinFile(os, "CCBF", 4);
    WriteIntToBinFile(os, kVersion, 2);
    WriteIntToBinFile(os, kK, 2);
    WriteIntToBinFile(os, kM, 4);
    WriteBitArrToBinFile(os);
}

int BloomFilter::LoadBinary(std::ifstream& is) {
    if (!AreNextNBytesFromFileMatchStr(is, "CCBF", 4)) {
        return 1;
    }

    if (!AreNextNBytesFromFileMatchInt(is, kVersion, 2) || 
        !AreNextNBytesFromFileMatchInt(is, kK, 2) || 
        !AreNextNBytesFromFileMatchInt(is, kM, 4)) {
        return 2;
    }

    LoadBitArrFromBinFile(is);
    return 0;
}

void BloomFilter::WriteStrToBinFile(std::ofstream& os, const std::string& str, int size_in_bytes) {
    char buffer[size_in_bytes];
    std::memcpy(buffer, &str[0], size_in_bytes);
    os.write(buffer, size_in_bytes);
}

void BloomFilter::WriteIntToBinFile(std::ostream& os, int n, int size_in_bytes) {
    char buffer[size_in_bytes];
    std::memcpy(buffer, &n, size_in_bytes);
    os.write(buffer, size_in_bytes);
}

void BloomFilter::WriteBitArrToBinFile(std::ostream& os) {
    int bit_array_size_in_bytes = (kM + 7) / 8;
    char buffer[bit_array_size_in_bytes];
    std::memcpy(buffer, &bit_array_, bit_array_size_in_bytes);
    os.write(buffer, bit_array_size_in_bytes);
}

bool BloomFilter::AreNextNBytesFromFileMatchStr(std::istream& is, const std::string& str, int n) {
    // Terminating null is required for string comparison
    char buffer[n + 1] = { '\0' };
    is.read(buffer, n);
    return std::strcmp(buffer, str.c_str()) == 0;
}

bool BloomFilter::AreNextNBytesFromFileMatchInt(std::istream& is, int dest, int n) {
    char buffer[n];
    is.read(buffer, n);

    int src = 0;
    std::memcpy(&src, buffer, n);

    return src == dest;
}

void BloomFilter::LoadBitArrFromBinFile(std::istream& is) {
    int bit_array_size_in_bytes = (kM + 7) / 8;
    char buffer[bit_array_size_in_bytes];
    is.read(buffer, sizeof(buffer));
    std::memcpy(&bit_array_, buffer, sizeof(buffer));
}
