#include "Field.h"
#include "CompressedFieldStorage.h"

Field::Field()
    : width_(0), height_(0), storage_(nullptr)
{}

bool Field::TrySetSize(std::size_t width, std::size_t height) {
    if (!storage_) {
        storage_ = new CompressedFieldStorage();
    }
    if (!storage_->TrySetSize(width, height)) {
        return false;
    }
    width_ = width;
    height_ = height;
    return true;
}

ShotResult Field::Shot(std::size_t x, std::size_t y) {
    if (x >= width_ || y >= height_) {
        std::cerr << "Shot out of range";
    }
    CellState temp_cell_state = storage_->GetCell(x, y);
    if (temp_cell_state == CellState::Ship) {
        storage_->SetCell(x, y, CellState::Hit);
        return ShotResult::kKill;
    }
    if (temp_cell_state == CellState::Empty) {
        storage_->SetCell(x, y, CellState::Miss);
        return ShotResult::kMiss;
    }
    return ShotResult::kMiss;
}

bool Field::TryPlaceShip(const Ship& ship) {
    std::size_t x = ship.GetX();
    std::size_t y = ship.GetY();
    bool horizontal = (ship.GetOrientation() == Orientation::kHorizontal);

    for (std::size_t i = 0; i < ship.GetSize(); i++) {
        std::size_t temp_x = x + (horizontal ? i : 0);
        std::size_t temp_y = y + (horizontal ? 0 : i);
        if (temp_x >= width_ || temp_y >= height_) {
            return false;
        }
        if (storage_->GetCell(temp_x, temp_y) != CellState::Empty) {
            return false;
        }
    }

    for (std::size_t i = 0; i < ship.GetSize(); i++) {
        std::size_t temp_x = x + (horizontal ? i : 0);
        std::size_t temp_y = y + (horizontal ? 0 : i);
        storage_->SetCell(temp_x, temp_y, CellState::Ship);
        loaded_ships_.push_back(ship);
    }

    return true;
}

std::vector<Ship> Field::GetAllShips() const {
    return loaded_ships_;
}

bool Field::TryDump(const char* path) {
    std::ofstream ofs(path);
    if (!ofs) {
        return false;
    }

    ofs << width_ << " " << height_ << "\n";

    for (const auto& ship : GetAllShips()) {
        char orientation = (ship.GetOrientation() == Orientation::kHorizontal) ? 'h' : 'v';
        ofs << ship.GetSize() << " " << orientation << " " << ship.GetX() << " " << ship.GetY() << "\n";
    }

    return true;
}

bool Field::TryLoad(const char* path) {
    std::ifstream ifs(path);
    if (!ifs) {
        return false;
    }

    std::size_t width, height;
    if (!(ifs >> width >> height)) {
        return false;
    }

    if (!TrySetSize(width, height)) {
        return false;
    }

    loaded_ships_.clear();
    int size;
    char orientation;
    std::size_t x, y;

    while (ifs >> size >> orientation >> x >> y) {
        if (orientation != 'h' && orientation != 'v') {
            return false;
        }
        Ship ship(size, x, y, (orientation == 'h') ? Orientation::kHorizontal : Orientation::kVertical);
        loaded_ships_.push_back(ship);
    }

    return true;
}

std::size_t Field::GetWidth() const {
    return width_;
}

std::size_t Field::GetHeight() const {
    return height_;
}

void Field::SetStorage(IFieldStorage* storage) {
    storage_ = storage;
}


