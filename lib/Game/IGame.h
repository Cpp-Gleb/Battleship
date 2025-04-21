#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <fstream>

enum class PlayerRole {
    Master,
    Slave
};

class IGame {
public:
    virtual ~IGame() = default;

    virtual bool TryCreate(PlayerRole role) = 0;
    virtual bool TrySetWidth(uint64_t w) = 0;
    virtual bool TrySetHeight(uint64_t h) = 0;
    virtual uint64_t GetWidth() const = 0;
    virtual uint64_t GetHeight() const = 0;
    virtual bool TrySetCount(std::size_t shipType, uint64_t count) = 0;
    virtual uint64_t GetCount(std::size_t shipType) const = 0;
    virtual bool TrySetStrategy(const std::string& strat) = 0;
    virtual bool TryStart() = 0;
    virtual bool TryStop() = 0;
    virtual std::string IncomingShot(uint64_t x, uint64_t y) = 0;
    virtual std::string GetNextShot() = 0;
    virtual bool TrySetResult(const std::string& res) = 0;
    virtual std::string Finished() const = 0;
    virtual std::string Win() const = 0;
    virtual std::string Lose() const = 0;
    virtual bool TryDump(const char* path) = 0;
    virtual bool TryLoad(const char* path) = 0;

    // virtual void DebugPrintState() const = 0;
};

