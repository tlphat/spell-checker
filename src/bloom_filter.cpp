#include "bloom_filter.h"
#include "hash_utilities.h"

const std::array<unsigned int, BloomFilter::K> BloomFilter::knv_offsets = {
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

const std::array<unsigned int, BloomFilter::K> BloomFilter::knv_primes = {
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
    for (int i = 0; i < K; ++i) {
        hash_functions[i] = [i](const std::string& s) -> int { 
            return hash::base_fnv1(s, knv_offsets[i], knv_primes[i], M); 
        };
    }
}

void BloomFilter::add(const std::string& word) {
    for (HashFunction hash_function : hash_functions) {
        int i = hash_function(word);
        bit_array[i] = 1;
    }
}

bool BloomFilter::exists(const std::string& word) {
    for (HashFunction hash_function : hash_functions) {
        int i = hash_function(word);
        if (bit_array[i] == 0) {
            return false;
        }
    }
    return true;
}
