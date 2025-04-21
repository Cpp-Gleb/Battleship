#pragma once

#include <cstddef>

enum class Orientation {
    kHorizontal = 0,
    kVertical
};

class Ship {
public:
    Ship(std::size_t size, std::size_t x, std::size_t y, Orientation orientation);

    void Hit();
    [[nodiscard]] bool IsAlive() const;
    [[nodiscard]] bool ContainsPoint(std::size_t cell_x, std::size_t cell_y) const;

    [[nodiscard]] std::size_t GetSize() const;
    [[nodiscard]] std::size_t GetX() const;
    [[nodiscard]] std::size_t GetY() const;
    [[nodiscard]] Orientation GetOrientation() const;

private:
    std::size_t size_;
    std::size_t x_;
    std::size_t y_;
    Orientation orientation_;
    std::size_t alive_cells_;
};