#include "HuntingManager.h"

HuntingManager::HuntingManager() {}

bool HuntingManager::HasTarget() const {
    return !found_ship_cells_.empty();
}

void HuntingManager::StartHunt(std::pair<std::size_t,std::size_t> shot, const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH]) {
    found_ship_cells_.clear();
    found_ship_cells_.push_back(shot);
}

void HuntingManager::AddHit(std::pair<std::size_t,std::size_t> shot) {
    found_ship_cells_.push_back(shot);
}

void HuntingManager::OnMiss(std::pair<std::size_t,std::size_t> shot, const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH]) {
    (void)shot; (void)map;
}

std::optional<std::pair<std::size_t,std::size_t>> HuntingManager::GetHuntingShot(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH]) {
    if (found_ship_cells_.empty()) {
        return std::nullopt;
    }

    auto [minx, maxx, miny, maxy] = ComputeMinMax();
    bool horizontal = (miny == maxy);
    bool vertical = (minx == maxx);

    if (found_ship_cells_.size() == 1) {
        return ShotAroundSingleHit(map);
    }

    if (horizontal) {
        return HorizontalExtension(map, minx, maxx, miny);
    }

    if (vertical) {
        return VerticalExtension(map, miny, maxy, minx);
    }

    return AroundFirstCell(map);
}

void HuntingManager::MarkNoShipAround(EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH]) {
    if (found_ship_cells_.empty()) return;

    auto [minx, maxx, miny, maxy] = ComputeMinMax();
    for (std::size_t yy = (miny > 0 ? miny - 1 : miny); yy <= (maxy + 1 < KLOCAL_HEIGHT ? maxy + 1 : maxy); yy++) {
        for (std::size_t xx = (minx > 0 ? minx - 1 : minx); xx <= (maxx + 1 < KLOCAL_WIDTH ? maxx + 1 : maxx); xx++) {
            if (map[yy][xx] == EnemyCellState::Unknown) {
                map[yy][xx] = EnemyCellState::NoShip;
            }
        }
    }
}

void HuntingManager::Reset() {
    found_ship_cells_.clear();
}

std::size_t HuntingManager::GetFoundShipSize() const {
    return found_ship_cells_.size();
}

std::tuple<std::size_t,std::size_t,std::size_t,std::size_t> HuntingManager::ComputeMinMax() const {
    std::size_t minx = std::numeric_limits<std::size_t>::max();
    std::size_t maxx = 0;
    std::size_t miny = std::numeric_limits<std::size_t>::max();
    std::size_t maxy = 0;

    for (auto &cell : found_ship_cells_) {
        minx = std::min(minx, cell.first);
        maxx = std::max(maxx, cell.first);
        miny = std::min(miny, cell.second);
        maxy = std::max(maxy, cell.second);
    }
    return {minx, maxx, miny, maxy};
}
std::optional<std::pair<std::size_t, std::size_t>> HuntingManager::ShotAroundSingleHit(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH])
{
    auto [x, y] = found_ship_cells_[0];
    std::vector<std::pair<int, int>> dirs = {
        {0, 1},
        {0, -1},
        {1, 0},
        {-1, 0}
    };

    for (auto &d : dirs) {
        int nx = static_cast<int>(x) + d.first;
        int ny = static_cast<int>(y) + d.second;
        if (nx >= 0 && nx < static_cast<int>(KLOCAL_WIDTH) &&
            ny >= 0 && ny < static_cast<int>(KLOCAL_HEIGHT))
        {
            if (map[ny][nx] == EnemyCellState::Unknown) {
                return std::make_pair(static_cast<std::size_t>(nx), static_cast<std::size_t>(ny));
            }
        }
    }
    return std::nullopt;
}

std::optional<std::pair<std::size_t, std::size_t>> HuntingManager::HorizontalExtension(
    const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH],
    std::size_t minx, std::size_t maxx, std::size_t y)
{
    if (minx > 0 && map[y][minx - 1] == EnemyCellState::Unknown) {
        return std::make_pair(minx - 1, y);
    }
    if (maxx + 1 < KLOCAL_WIDTH && map[y][maxx + 1] == EnemyCellState::Unknown) {
        return std::make_pair(maxx + 1, y);
    }
    return std::nullopt;
}

std::optional<std::pair<std::size_t, std::size_t>> HuntingManager::VerticalExtension(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH], std::size_t miny, std::size_t maxy, std::size_t x)
{
    if (miny > 0 && map[miny - 1][x] == EnemyCellState::Unknown) {
        return std::make_pair(x, miny - 1);
    }
    if (maxy + 1 < KLOCAL_HEIGHT && map[maxy + 1][x] == EnemyCellState::Unknown) {
        return std::make_pair(x, maxy + 1);
    }
    return std::nullopt;
}

std::optional<std::pair<std::size_t, std::size_t>> HuntingManager::AroundFirstCell(const EnemyCellState (&map)[KLOCAL_HEIGHT][KLOCAL_WIDTH])
{
    auto [x, y] = found_ship_cells_[0];
    std::vector<std::pair<int, int>> dirs = {
        {0, 1},
        {0, -1},
        {1, 0},
        {-1, 0}
    };

    for (auto &d : dirs) {
        int nx = static_cast<int>(x) + d.first;
        int ny = static_cast<int>(y) + d.second;
        if (nx >= 0 && nx < static_cast<int>(KLOCAL_WIDTH) &&
            ny >= 0 && ny < static_cast<int>(KLOCAL_HEIGHT))
        {
            if (map[ny][nx] == EnemyCellState::Unknown) {
                return std::make_pair(static_cast<std::size_t>(nx), static_cast<std::size_t>(ny));
            }
        }
    }
    return std::nullopt;
}