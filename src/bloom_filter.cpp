#include "bloom_filter.h"
#include "hash_utilities.h"

#include <cstring>
#include <algorithm>
#include <iostream>
#include <bit>

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

void BloomFilter::LoadDictionary(std::ifstream& is) {
    std::string line;
    while (std::getline(is, line)) {
        Add(line);
    }
}

void BloomFilter::SaveBinary(std::ofstream& os) {
    char identifier[4];
    std::memcpy(identifier, &"CCBF", sizeof(identifier));
    os.write(identifier, sizeof(identifier));

    char version[2];
    std::memcpy(version, &kVersion, sizeof(version));
    os.write(version, sizeof(version));

    char k[2];
    std::memcpy(k, &kK, sizeof(k));
    os.write(k, sizeof(k));

    char m[4];
    std::memcpy(m, &kM, sizeof(m));
    os.write(m, sizeof(m));

    const int bit_array_size_in_bytes = (kM + 7) / 8;
    char bit_array[bit_array_size_in_bytes];
    std::memcpy(bit_array, &bit_array_, sizeof(bit_array));
    os.write(bit_array, sizeof(bit_array));
}

void BloomFilter::LoadBinary(std::ifstream& is) {

}
