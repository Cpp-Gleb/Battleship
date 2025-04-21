#include "CompressedFieldStorage.h"

CompressedFieldStorage::CompressedFieldStorage()
    : width_(0),
      height_(0),
      zonesX_(0),
      zonesY_(0),
      zones_(nullptr),
      current_zone_x_(0),
      current_zone_y_(0) {}

CompressedFieldStorage::~CompressedFieldStorage() {
    FreeZones();
}

bool CompressedFieldStorage::TrySetSize(std::size_t width, std::size_t height) {
    FreeZones();
    width_ = width;
    height_ = height;
    zonesX_ = (width_ + kZONE_WIDTH - 1) / kZONE_WIDTH;
    zonesY_ = (height_ + kZONE_HEIGHT - 1) / kZONE_HEIGHT;
    AllocateZones();
    LoadZone(0, 0);
    return true;
}

CellState CompressedFieldStorage::GetCell(std::size_t x, std::size_t y) {
    if (x >= width_ || y >= height_) {
        std::cerr << "GetCell out of range";
    }
    
    std::size_t zone_x;
    std::size_t zone_y;
    std::size_t local_x;
    std::size_t local_y;
    
    GetZoneCoords(x, y, zone_x, zone_y, local_x, local_y);
    if (zone_x != current_zone_x_ || zone_y != current_zone_y_) {
        UnloadCurrentZone();
        LoadZone(zone_x, zone_y);
    }
    
    std::size_t idx = local_y * zones_[ZoneIndex(zone_x, zone_y)].zoneW + local_x;
    unsigned char value = zones_[ZoneIndex(zone_x, zone_y)].current_data[idx];
    switch (value) {
        case 0: return CellState::Empty;
        case 1: return CellState::Ship;
        case 2: return CellState::Hit;
        case 3: return CellState::Miss;
        default: return CellState::Empty;
    }
}

void CompressedFieldStorage::SetCell(std::size_t x, std::size_t y, CellState state) {
    if (x >= width_ || y >= height_) {
        std::cerr << ("SetCell out of range");
    }
    
    std::size_t zone_x;
    std::size_t zone_y;
    std::size_t local_x;
    std::size_t local_y;
    
    GetZoneCoords(x, y, zone_x, zone_y, local_x, local_y);
    if (zone_x != current_zone_x_ || zone_y != current_zone_y_) {
        UnloadCurrentZone();
        LoadZone(zone_x, zone_y);
    }
    
    auto& zone_it = zones_[ZoneIndex(zone_x, zone_y)];
    std::size_t idx = local_y * zone_it.zoneW + local_x;
    unsigned char value = 0;
    switch (state) {
        case CellState::Empty:
            value = 0;
            break;
        case CellState::Ship:
            value = 1;
            break;
        case CellState::Hit:
            value = 2;
            break;
        case CellState::Miss:
            value = 3;
            break;
    }
    
    zone_it.current_data[idx] = value;
    zone_it.is_modified = true;
}

bool CompressedFieldStorage::TrySaveToFile(const char* path) const {
    (void)path;
    return true;
}

bool CompressedFieldStorage::TryLoadFromFile(const char* path) {
    (void)path;
    return true;
}

void CompressedFieldStorage::AllocateZones() {
    zones_ = new ZoneInfo[zonesX_ * zonesY_];
    for (std::size_t zone_y = 0; zone_y < zonesY_; zone_y++) {
        for (std::size_t zone_x = 0; zone_x < zonesX_; zone_x++) {
            auto& zone_it = zones_[ZoneIndex(zone_x, zone_y)];
            std::size_t width = (zone_x == zonesX_ - 1) ? (width_ - zone_x * kZONE_WIDTH) : kZONE_WIDTH;
            std::size_t height = (zone_y == zonesY_ - 1) ? (height_ - zone_y * kZONE_HEIGHT) : kZONE_HEIGHT;
            zone_it.zoneW = width;
            zone_it.zoneH = height;
            zone_it.compressedData = nullptr;
            zone_it.compressedSize = 0;
            zone_it.is_loaded = false;
            zone_it.is_modified = false;
            for (std::size_t i = 0; i < width * height; i++) {
                zone_it.current_data[i] = 0;
            }
            
            std::vector<unsigned char> raw(width * height, 0);
            auto compressed = compressor_.Compress(raw.data(), width * height);
            zone_it.compressedSize = compressed.size();
            zone_it.compressedData = new unsigned char[zone_it.compressedSize];
            std::copy(compressed.begin(), compressed.end(), zone_it.compressedData);
        }
    }
}

void CompressedFieldStorage::FreeZones() {
    if (zones_) {
        for (std::size_t i = 0; i < zonesX_ * zonesY_; i++) {
            if (zones_[i].compressedData) {
                delete[] zones_[i].compressedData;
            }
        }
        
        delete[] zones_;
        zones_ = nullptr;
    }
    
    width_ = 0;
    height_ = 0;
    zonesX_ = 0;
    zonesY_ = 0;
}

std::size_t CompressedFieldStorage::ZoneIndex(std::size_t zone_x, std::size_t zone_y) const {
    return zone_y * zonesX_ + zone_x;
}

void CompressedFieldStorage::GetZoneCoords(std::size_t x, std::size_t y, std::size_t& zone_x, std::size_t& zone_y, std::size_t& localocal_x, std::size_t& localocal_y) const {
    zone_x = x / kZONE_WIDTH;
    zone_y = y / kZONE_HEIGHT;
    localocal_x = x % kZONE_WIDTH;
    localocal_y = y % kZONE_HEIGHT;
}

void CompressedFieldStorage::LoadZone(std::size_t zone_x, std::size_t zone_y) const {
    current_zone_x_ = zone_x;
    current_zone_y_ = zone_y;
    DecompressZone(zone_x, zone_y);
    zones_[ZoneIndex(zone_x, zone_y)].is_loaded = true;
    zones_[ZoneIndex(zone_x, zone_y)].is_modified = false;
}

void CompressedFieldStorage::CompressZone(std::size_t zone_x, std::size_t zone_y) {
    auto& zone_it = zones_[ZoneIndex(zone_x, zone_y)];
    std::size_t size = zone_it.zoneW * zone_it.zoneH;
    std::vector<unsigned char> raw;
    raw.reserve(size);
    
    for (std::size_t i = 0; i < size; i++) {
        raw.push_back(zone_it.current_data[i]);
    }
    
    auto compressed = compressor_.Compress(raw.data(), size);
    if (zone_it.compressedData) {
        delete[] zone_it.compressedData;
    }
    
    zone_it.compressedSize = compressed.size();
    zone_it.compressedData = new unsigned char[zone_it.compressedSize];
    std::copy(compressed.begin(), compressed.end(), zone_it.compressedData);
    zone_it.is_modified = false;
}

void CompressedFieldStorage::UnloadCurrentZone() {
    auto& zone_it = zones_[ZoneIndex(current_zone_x_, current_zone_y_)];
    if (zone_it.is_modified) {
        CompressZone(current_zone_x_, current_zone_y_);
    }
    
    zone_it.is_loaded = false;
}

void CompressedFieldStorage::DecompressZone(std::size_t zone_x, std::size_t zone_y) const {
    auto& zone_it = const_cast<ZoneInfo&>(zones_[ZoneIndex(zone_x, zone_y)]);
    std::size_t size = zone_it.zoneW * zone_it.zoneH;
    std::vector<unsigned char> decompressed = compressor_.Decompress(zone_it.compressedData, zone_it.compressedSize, size);
    for (std::size_t i = 0; i < size; i++) {
        zone_it.current_data[i] = decompressed[i];
    }
}

