#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <cstdlib>
#include <ctime>

#include "IStrategy.h"
#include "Statistics/Statistics.h"
#include "Zone/IZoneManager.h"
#include "Pattern/IPatternManager.h"
#include "Hunting/IHuntingManager.h"

class CustomStrategy : public IStrategy {
public:

    CustomStrategy(uint64_t battleships, uint64_t cruisers, uint64_t destroyers, uint64_t submarines, IZoneManager* zoneManager, IPatternManager* patternManager, IHuntingManager* huntingManager, Statistics* stats);

    std::pair<std::size_t, std::size_t> GetNextShot() override;
    void UpdateStrategy(std::pair<std::size_t,std::size_t> shot, ShotResult result) override;

private:
    uint64_t battleships_;
    uint64_t cruisers_;
    uint64_t destroyers_;
    uint64_t submarines_;

    Phase game_phase_;

    EnemyCellState enemy_map_[KLOCAL_HEIGHT][KLOCAL_WIDTH];

    IZoneManager* zone_manager_;
    IPatternManager* pattern_manager_;
    IHuntingManager* hunting_manager_;
    Statistics* stats_;

    void UpdateCurrentPatternType();
    void IdentifySunkShip(std::size_t size);
};
