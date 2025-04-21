#pragma once

#include <cstdlib>

#include "IPatternManager.h"
#include "CustomStrategy/Zone/IZoneManager.h"

class PatternManager : public IPatternManager {
public:
    PatternManager();

    void SetPatternForTarget(const std::string& target) override;
    std::string GetCurrentPatternType() const override;
    void ResetPatternIndex() override;

    std::pair<std::size_t,std::size_t> GetNextShot(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH], const IZoneManager& zone_manager, const std::string& patternType) override;

private:
    std::size_t pattern_index_;
    std::string current_pattern_type_;

    std::pair<std::size_t,std::size_t> GetBattleshipPatternShot(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH]);
    std::pair<std::size_t,std::size_t> GetCruiserPatternShot(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH]);
    std::pair<std::size_t,std::size_t> GetDestroyerPatternShot(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH]);
    std::pair<std::size_t,std::size_t> GetRandomForSubmarines(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH]);
};