#include "hash_utilities.h"

namespace hash {

unsigned int base_fnv1(const std::string& data, unsigned int offset, unsigned int prime, int mod) {
    unsigned int res = offset;
    for (char byte : data) {
        res *= prime;
        res ^= byte;
    }
    return res % mod;
}

}
