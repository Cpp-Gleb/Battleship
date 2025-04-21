#pragma once

#include <cstddef>

enum class CellState {
    Empty,
    Ship,
    Hit,
    Miss
};

class IFieldStorage {
public:
    virtual ~IFieldStorage() = default;
    virtual bool TrySetSize(std::size_t width, std::size_t height) = 0;
    virtual CellState GetCell(std::size_t x, std::size_t y) = 0;
    virtual void SetCell(std::size_t x, std::size_t y, CellState state) = 0;
    virtual bool TrySaveToFile(const char* path) const = 0;
    virtual bool TryLoadFromFile(const char* path) = 0;
};

