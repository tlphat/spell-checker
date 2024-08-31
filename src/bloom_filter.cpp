#include "bloom_filter.h"
#include "hash_utilities.h"

#include <cstring>
#include <algorithm>
#include <bit>

#include <iostream>

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

void BloomFilter::LoadDictionary(std::ifstream& is) {
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
    WriteBitArrToBinFile(os, bit_array_, (kM + 7) / 8);
}

int BloomFilter::LoadBinary(std::ifstream& is) {
    if (!AreNextNBytesFromFileMatchStr(is, "CCBF", 4)) {
        return 1;
    }
    
    char version[2];
    is.read(version, sizeof(version));

    char k[2];
    is.read(k, sizeof(k));

    char m[4];
    is.read(m, sizeof(m));

    char bit_arr[(kM + 7) / 8];
    is.read(bit_arr, sizeof(bit_arr));
    std::memcpy(&bit_array_, bit_arr, (kM + 7) / 8);

    return 0;
}

void BloomFilter::WriteStrToBinFile(std::ofstream& os, const std::string& str, int size_in_bytes) {
    char arr[size_in_bytes];
    std::memcpy(arr, &str[0], size_in_bytes);
    os.write(arr, size_in_bytes);
}

void BloomFilter::WriteIntToBinFile(std::ostream& os, int n, int size_in_bytes) {
    char arr[size_in_bytes];
    std::memcpy(arr, &n, size_in_bytes);
    os.write(arr, size_in_bytes);
}

void BloomFilter::WriteBitArrToBinFile(std::ostream& os, const BitArray& bit_array, int size_in_bytes) {
    char arr[size_in_bytes];
    std::memcpy(arr, &bit_array, size_in_bytes);
    os.write(arr, size_in_bytes);
}

bool BloomFilter::AreNextNBytesFromFileMatchStr(std::istream& is, const std::string& str, int n) {
    // Terminating null is required for string comparison
    char arr[n + 1] = { '\0' };
    is.read(arr, n);
    return std::strcmp(arr, str.c_str()) == 0;
}

bool BloomFilter::AreNextNBytesFromFileMatchInt(std::istream& is, int target, int n) {
    return true;
}
