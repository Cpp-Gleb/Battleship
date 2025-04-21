#pragma once

#include <optional>

#include "IStrategy.h"

class IHuntingManager {
public:
    virtual ~IHuntingManager() = default;
    virtual bool HasTarget() const = 0;
    virtual void StartHunt(std::pair<std::size_t,std::size_t> shot, const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH]) = 0;
    virtual void AddHit(std::pair<std::size_t,std::size_t> shot) = 0;
    virtual void OnMiss(std::pair<std::size_t,std::size_t> shot, const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH]) = 0;
    virtual std::optional<std::pair<std::size_t,std::size_t>> GetHuntingShot(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH]) = 0;
    virtual void MarkNoShipAround(EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH]) = 0;
    virtual void Reset() = 0;
    virtual std::size_t GetFoundShipSize() const = 0;
};