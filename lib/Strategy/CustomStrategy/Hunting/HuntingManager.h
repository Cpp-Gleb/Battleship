#pragma once

#include <limits>
#include <algorithm>
#include <vector>
#include <tuple>

#include "IHuntingManager.h"
#include "CustomStrategy/CustomStrategy.h"

class HuntingManager : public IHuntingManager {
public:
    HuntingManager();

    bool HasTarget() const override;
    void StartHunt(std::pair<std::size_t,std::size_t> shot, const EnemyCellState (&map)[10][10]) override;
    void AddHit(std::pair<std::size_t,std::size_t> shot) override;
    void OnMiss(std::pair<std::size_t,std::size_t> shot, const EnemyCellState (&map)[10][10]) override;
    std::optional<std::pair<std::size_t,std::size_t>> GetHuntingShot(const EnemyCellState (&map)[10][10]) override;
    void MarkNoShipAround(EnemyCellState (&map)[10][10]) override;
    void Reset() override;
    std::size_t GetFoundShipSize() const override;

private:
    std::vector<std::pair<std::size_t,std::size_t>> found_ship_cells_;
    std::tuple<std::size_t,std::size_t,std::size_t,std::size_t> ComputeMinMax() const;
    std::optional<std::pair<std::size_t, std::size_t>> ShotAroundSingleHit(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH]);
    std::optional<std::pair<std::size_t, std::size_t>> HorizontalExtension(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH], std::size_t minx, std::size_t maxx, std::size_t y);
    std::optional<std::pair<std::size_t, std::size_t>> VerticalExtension(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH], std::size_t miny, std::size_t maxy, std::size_t x);
    std::optional<std::pair<std::size_t, std::size_t>> AroundFirstCell(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH]);
};
