#pragma once
#include <cstddef>
#include "Field.h"

static const std::size_t KLOCAL_WIDTH = 10;
static const std::size_t KLOCAL_HEIGHT = 10;

enum class EnemyCellState {
    Unknown,
    Miss,
    Hit,
    NoShip
};

enum class Phase {
    Searching,
    Hunting
};

class IStrategy {
public:
    virtual ~IStrategy() = default;
    virtual std::pair<std::size_t, std::size_t> GetNextShot() = 0;
    virtual void UpdateStrategy(std::pair<std::size_t, std::size_t> shot, ShotResult result) = 0;
};
