#pragma once

#include <cstddef>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <fstream>


#include "IFieldStorage.h"
#include "FieldCompressor.h"

class CompressedFieldStorage : public IFieldStorage {
public:
    CompressedFieldStorage();
    ~CompressedFieldStorage();

    bool TrySetSize(std::size_t width, std::size_t height) override;
    CellState GetCell(std::size_t x, std::size_t y) override;
    void SetCell(std::size_t x, std::size_t y, CellState state) override;
    bool TrySaveToFile(const char* path) const override;
    bool TryLoadFromFile(const char* path) override;

private:
    static const std::size_t kZONE_WIDTH = 10;
    static const std::size_t kZONE_HEIGHT = 10;

    std::size_t width_;
    std::size_t height_;
    std::size_t zonesX_;
    std::size_t zonesY_;

    struct ZoneInfo {
        unsigned char* compressedData;
        std::size_t compressedSize;
        bool is_loaded;
        bool is_modified;
        std::size_t zoneW;
        std::size_t zoneH;
        unsigned char current_data[kZONE_WIDTH * kZONE_HEIGHT];
    };

    ZoneInfo* zones_;
    FieldCompressor compressor_;

    mutable std::size_t current_zone_x_;
    mutable std::size_t current_zone_y_;

    void AllocateZones();
    void FreeZones();
    std::size_t ZoneIndex(std::size_t zx, std::size_t zy) const;
    void GetZoneCoords(std::size_t x, std::size_t y, std::size_t& zx, std::size_t& zy, std::size_t& localX, std::size_t& localY) const;
    void LoadZone(std::size_t zx, std::size_t zy) const;
    void UnloadCurrentZone();
    void CompressZone(std::size_t zx, std::size_t zy);
    void DecompressZone(std::size_t zx, std::size_t zy) const;
};

