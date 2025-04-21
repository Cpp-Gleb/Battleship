#pragma once

#include <cstddef>
#include <vector>

class FieldCompressor {
public:
    FieldCompressor();
    ~FieldCompressor();
    std::vector<unsigned char> Compress(const unsigned char* data, std::size_t size) const;
    std::vector<unsigned char> Decompress(const unsigned char* data, std::size_t size, std::size_t outSize) const;
};

