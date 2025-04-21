#include "Ship.h"

Ship::Ship(std::size_t size, std::size_t x, std::size_t y, Orientation orientation)
    : size_(size),
      x_(x),
      y_(y),
      orientation_(orientation),
      alive_cells_(size) {}

void Ship::Hit() {
    alive_cells_--;
}

bool Ship::IsAlive() const {
    return alive_cells_ > 0;
}

bool Ship::ContainsPoint(std::size_t cell_x, std::size_t cell_y) const {
    if (orientation_ == Orientation::kHorizontal) {
        return (cell_y == y_) && (cell_x >= x_) && (cell_x < x_ + size_);
    } 
    return (cell_x == x_) && (cell_y >= y_) && (cell_y < y_ + size_);
}

std::size_t Ship::GetSize() const { return size_; }
std::size_t Ship::GetX() const { return x_; }
std::size_t Ship::GetY() const { return y_; }
Orientation Ship::GetOrientation() const { return orientation_; }