# Spell Checker

A spell checker using Bloom filter.

The Unix-based word dictionary (235,976 words) is used. 
To achieve 0.01% false positive rate, the bit array size is chosen to be 4.6M and the number of hash functions is chosen to be 17.

## Reference

[Bloom filter - Wikipedia](https://en.wikipedia.org/wiki/Bloom_filter)