#pragma once

#include <vector>

#include "IZoneManager.h"
#include "CustomStrategy/Statistics/Statistics.h"

class ZoneManager : public IZoneManager {
public:
    ZoneManager();

    void SwitchToNextZone() override;
    bool NeedZoneSwitch(Phase phase, const Statistics& stats) const override;
    void UpdateZoneState(std::size_t x, std::size_t y, ShotResult result) override;


private:
    std::size_t shots_;
    std::size_t zoneIndex_;
};
