#pragma once

#include <cstddef>

#include "IStrategy.h"
#include "Statistics/Statistics.h"

class IZoneManager {
public:
    virtual ~IZoneManager() = default;
    virtual void SwitchToNextZone() = 0;
    virtual bool NeedZoneSwitch(Phase phase, const Statistics& stats) const = 0;
    virtual void UpdateZoneState(std::size_t x, std::size_t y, ShotResult result) = 0;
};