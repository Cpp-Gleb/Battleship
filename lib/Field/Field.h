#pragma once

#include <vector>
#include <iostream>
#include "Ship.h"
#include "IFieldStorage.h"

enum class ShotResult {
    kMiss = 0,
    kHit,
    kKill
};

class Field {
public:
    Field();
    bool TrySetSize(std::size_t width, std::size_t height);
    ShotResult Shot(std::size_t x, std::size_t y);
    bool TryPlaceShip(const Ship& ship);
    bool TryDump(const char* path);
    bool TryLoad(const char* path);
    std::size_t GetWidth() const;
    std::size_t GetHeight() const;

    void SetStorage(IFieldStorage* storage);

    const std::vector<Ship>& GetLoadedShips() const;
    std::vector<Ship> GetAllShips() const;
    std::size_t GetLoadedWidth() const;
    std::size_t GetLoadedHeight() const;

private:
    std::size_t width_;
    std::size_t height_;
    IFieldStorage* storage_;

    std::size_t loaded_width_;
    std::size_t loaded_height_;
    std::vector<Ship> loaded_ships_;
};