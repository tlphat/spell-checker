#!/bin/sh

g++ -std=c++17 -o build/spellchecker src/main.cpp \
    src/bloom_filter.cpp src/hash_utilities.cpp
