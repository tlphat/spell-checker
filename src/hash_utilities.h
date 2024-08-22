#ifndef HASH_UTILITIES_H
#define HASH_UTILITIES_H

#include <string>

namespace hash {

unsigned int base_fnv1(const std::string& data, unsigned int offset, unsigned int prime, int mod);

}

#endif
