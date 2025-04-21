#include "OrderedStrategy.h"

OrderedStrategy::OrderedStrategy(std::size_t width, std::size_t height)
    : width_(width), height_(height), current_x_(0), current_y_(0) {}

std::pair<std::size_t, std::size_t> OrderedStrategy::GetNextShot() {
    if (current_y_ >= height_) {
        return {0,0};
    }

    std::pair<std::size_t, std::size_t> shot = {current_x_, current_y_};

    current_x_++;
    if (current_x_ >= width_) {
        current_x_ = 0;
        current_y_++;
    }

    return shot;
}

void OrderedStrategy::UpdateStrategy(std::pair<std::size_t, std::size_t> shot, ShotResult result) {
    (void)shot;
    (void)result;
}
