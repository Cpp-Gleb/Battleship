#include "PatternManager.h"

PatternManager::PatternManager()
    : pattern_index_(0),
      current_pattern_type_("battleship") {}

void PatternManager::SetPatternForTarget(const std::string& target) {
    current_pattern_type_ = target;
}

std::string PatternManager::GetCurrentPatternType() const {
    return current_pattern_type_;
}

void PatternManager::ResetPatternIndex() {
    pattern_index_ = 0;
}

std::pair<std::size_t,std::size_t> PatternManager::GetNextShot(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH], const IZoneManager&, const std::string& patternType)
{
    if (patternType == "battleship") {
        return GetBattleshipPatternShot(map);
    }

    if (patternType == "cruiser") {
        return GetCruiserPatternShot(map);
    }

    if (patternType == "destroyer") {
        return GetDestroyerPatternShot(map);
    }

    return GetRandomForSubmarines(map);
}

std::pair<std::size_t,std::size_t> PatternManager::GetBattleshipPatternShot(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH])
{
    for (int attempt = 0; attempt < 500; attempt++) {
        std::size_t x = pattern_index_ % KLOCAL_HEIGHT;
        std::size_t y = (pattern_index_ / KLOCAL_WIDTH) % KLOCAL_WIDTH;
        pattern_index_++;
        if (((x + y) % 4 == 0) && map[y][x] == EnemyCellState::Unknown) {
            return {x,y};
        }
    }
    return {0,0};
}

std::pair<std::size_t,std::size_t> PatternManager::GetCruiserPatternShot(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH])
{
    for (int attempt = 0; attempt < 500; attempt++) {
        std::size_t x = pattern_index_ % KLOCAL_HEIGHT;
        std::size_t y = (pattern_index_ / KLOCAL_WIDTH) % KLOCAL_WIDTH;
        pattern_index_++;
        if (((x + y) % 3 == 0) && map[y][x] == EnemyCellState::Unknown) {
            return {x,y};
        }
    }
    return {0,0};
}

std::pair<std::size_t,std::size_t> PatternManager::GetDestroyerPatternShot(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH])
{
    for (int attempt = 0; attempt < 500; attempt++) {
        std::size_t x = pattern_index_ % KLOCAL_HEIGHT;
        std::size_t y = (pattern_index_ / KLOCAL_WIDTH) % KLOCAL_WIDTH;
        pattern_index_++;
        if (((x + y) % 2 == 0) && map[y][x] == EnemyCellState::Unknown) {
            return {x,y};
        }
    }
    return {0,0};
}

std::pair<std::size_t,std::size_t> PatternManager::GetRandomForSubmarines(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH])
{
    std::vector<std::pair<std::size_t,std::size_t>> unknownCells;
    for (std::size_t xx = 0; xx < KLOCAL_HEIGHT; xx++){
        for (std::size_t yy = 0; yy < KLOCAL_WIDTH; yy++){
            if (map[yy][xx] == EnemyCellState::Unknown) {
                unknownCells.push_back({xx,yy});
            }
        }
    }
    if (unknownCells.empty()) {
        return {0,0};
    }

    int r = std::rand() % static_cast<int>(unknownCells.size());
    return unknownCells[r];
}