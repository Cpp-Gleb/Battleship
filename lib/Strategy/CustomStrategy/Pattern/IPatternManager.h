#pragma once

#include <string>
#include "Zone/IZoneManager.h"
#include "IStrategy.h"

class IZoneManager;

class IPatternManager {
public:
    virtual ~IPatternManager() = default;
    virtual void SetPatternForTarget(const std::string& target) = 0;
    virtual std::string GetCurrentPatternType() const = 0;
    virtual void ResetPatternIndex() = 0;

    virtual std::pair<std::size_t,std::size_t> GetNextShot(
        const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH],
        const IZoneManager& zone_manager,
        const std::string& patternType) = 0;
};