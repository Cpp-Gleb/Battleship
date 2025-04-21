#include "FieldCompressor.h"

FieldCompressor::FieldCompressor() {}
FieldCompressor::~FieldCompressor() {}

std::vector<unsigned char> FieldCompressor::Compress(const unsigned char* data, std::size_t size) const {
    std::vector<unsigned char> result;
    if (size == 0) {
        return result;
    }
    std::size_t position = 0;
    while (position < size) {
        unsigned char val = data[position];
        std::size_t run = 1;
        while (position + run < size && data[position+run] == val && run < 255) {
            run++;
        }
        result.push_back(static_cast<unsigned char>(run));
        result.push_back(val);
        position += run;
    }
    return result;
}

std::vector<unsigned char> FieldCompressor::Decompress(const unsigned char* data, std::size_t size, std::size_t outSize) const {
    std::vector<unsigned char> result;
    result.reserve(outSize);
    std::size_t position = 0;
    while (position + 1 < size) {
        unsigned char run = data[position];
        unsigned char val = data[position + 1];
        for (std::size_t i = 0; i < run; i++) {
            result.push_back(val);
        }
        position += 2;
    }
    return result;
}
