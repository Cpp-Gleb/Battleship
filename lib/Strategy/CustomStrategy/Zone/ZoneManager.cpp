#include "ZoneManager.h"

ZoneManager::ZoneManager()
    : shots_(0),
      zoneIndex_(0) {}

void ZoneManager::SwitchToNextZone() {
    zoneIndex_++;
    shots_ = 0;
}

bool ZoneManager::NeedZoneSwitch(Phase phase, const Statistics& stats) const {
    if (phase == Phase::Searching && stats.GetHitRatio() < 0.1 && shots_ > 50) {
        return true;
    }
    return false;
}

void ZoneManager::UpdateZoneState(std::size_t, std::size_t, ShotResult) {
    shots_++;
}