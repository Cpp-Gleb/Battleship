#pragma once

#include <cstddef>
#include "IStrategy.h"

class OrderedStrategy : public IStrategy {
public:
    OrderedStrategy(std::size_t width, std::size_t height);

    std::pair<std::size_t, std::size_t> GetNextShot() override;
    void UpdateStrategy(std::pair<std::size_t, std::size_t> shot, ShotResult result) override;

private:
    std::size_t width_;
    std::size_t height_;
    std::size_t current_x_;
    std::size_t current_y_;
};
