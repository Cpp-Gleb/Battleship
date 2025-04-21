#include "SlaveGame.h"

SlaveGame::SlaveGame()
    : role_(PlayerRole::Slave),
      is_game_created_(false),
      is_game_started_(false),
      is_game_stopped_(false),
      is_game_finished_(false),
      are_we_winner_(false),
      are_we_loser_(false),
      width_(0),
      height_(0),
      strategy_(nullptr),
      current_strategy_("custom"),
      zone_manager_(new ZoneManager()),
      pattern_manager_(new PatternManager()),
      hunting_manager_(new HuntingManager()),
      stats_(new Statistics()) {}

SlaveGame::~SlaveGame() {
    if (strategy_) {
        delete strategy_;
    }
    delete zone_manager_;
    delete pattern_manager_;
    delete hunting_manager_;
    delete stats_;
}

bool SlaveGame::TryCreate(PlayerRole role) {
    if (is_game_started_ || is_game_created_) {
        return false;
    }
    if (role != PlayerRole::Slave) {
        return false;
    }
    role_ = role;
    is_game_created_ = true;
    return true;
}

bool SlaveGame::TrySetWidth(uint64_t w) {
    return false;
}

bool SlaveGame::TrySetHeight(uint64_t h) {
    return false;
}

uint64_t SlaveGame::GetWidth() const {
    return width_;
}

uint64_t SlaveGame::GetHeight() const {
    return height_;
}

bool SlaveGame::TrySetCount(std::size_t shipType, uint64_t count) {
    return false;
}

uint64_t SlaveGame::GetCount(std::size_t shipType) const {
    auto it = ship_counts_.find(shipType);
    if (it != ship_counts_.end()) {
        return it->second;
    }
    return 0;
}

bool SlaveGame::TrySetStrategy(const std::string& strat) {
    if (!is_game_created_) {
        return false;
    }
    if (strat != "ordered" && strat != "custom") {
        return false;
    }
    current_strategy_ = strat;
    return true;
}

bool SlaveGame::TryStart() {
    if (!is_game_created_ || is_game_started_) {
        return false;
    }
    if (!CanStartGame()) {
        return false;
    }
    if (!field_.TrySetSize(width_, height_)) {
        return false;
    }

    InitializeStrategy();

    if (!ValidateFieldAndShips()) {
        return false;
    }
    PlaceInitialShips();

    is_game_started_ = true;
    is_game_stopped_ = false;
    is_game_finished_ = false;
    are_we_winner_ = false;
    are_we_loser_ = false;
    return true;
}

bool SlaveGame::TryStop() {
    if (!is_game_started_) {
        return false;
    }
    is_game_stopped_ = true;
    return true;
}

std::string SlaveGame::IncomingShot(uint64_t x, uint64_t y) {
    if (!is_game_started_ || is_game_finished_) {
        return "miss";
    }
    ShotResult result = field_.Shot(x, y);
    switch (result) {
        case ShotResult::kMiss:
            return "miss";
        case ShotResult::kHit:
            return "hit";
        case ShotResult::kKill:
            is_game_finished_ = true;
            are_we_winner_ = false;
            are_we_loser_ = true;
            return "kill";
    }
    return "miss";
}

std::string SlaveGame::GetNextShot() {
    if (!is_game_started_ || is_game_finished_) {
        return "0 0";
    }
    auto shot = strategy_->GetNextShot();
    return std::to_string(shot.first) + " " + std::to_string(shot.second);
}

bool SlaveGame::TrySetResult(const std::string& result) {
    if (!is_game_started_ || is_game_finished_) {
        return false;
    }
    ShotResult shot_result;
    if (result == "miss") {
        shot_result = ShotResult::kMiss;
    } else if (result == "hit") {
        shot_result = ShotResult::kHit;
    } else if (result == "kill") {
        shot_result = ShotResult::kKill;
        is_game_finished_ = true;
        are_we_winner_ = true;
        are_we_loser_ = false;
    } else {
        return false;
    }
    strategy_->UpdateStrategy({0, 0}, shot_result);
    return true;
}

std::string SlaveGame::Finished() const {
    return is_game_finished_ ? "yes" : "no";
}

std::string SlaveGame::Win() const {
    if (!is_game_finished_) {
        return "no";
    }
    return are_we_winner_ ? "yes" : "no";
}

std::string SlaveGame::Lose() const {
    if (!is_game_finished_) {
        return "no";
    }
    return are_we_loser_ ? "yes" : "no";
}

bool SlaveGame::TryDump(const char* path) {
    return field_.TryDump(path);
}

bool SlaveGame::TryLoad(const char* path) {
    bool can_load = field_.TryLoad(path);
    if (can_load) {
        width_ = field_.GetWidth();
        height_ = field_.GetHeight();
        ship_counts_[1] = 1;
    }
    return can_load;
}

bool SlaveGame::CanStartGame() const {
    if (width_ == 0 || height_ == 0) {
        return false;
    }
    bool is_any_ship_alive = false;
    for (const auto& ships_count : ship_counts_) {
        if (ships_count.second > 0) {
            is_any_ship_alive = true;
            break;
        }
    }
    return is_any_ship_alive;
}

void SlaveGame::InitializeStrategy() {
    if (current_strategy_ == "ordered") {
        strategy_ = new OrderedStrategy(static_cast<std::size_t>(width_), static_cast<std::size_t>(height_));
    } else {
        strategy_ = new CustomStrategy(GetCount(4), GetCount(3), GetCount(2), GetCount(1), zone_manager_, pattern_manager_, hunting_manager_, stats_);
    }
}

bool SlaveGame::ValidateFieldAndShips() const {
    if (width_ == 0 || height_ == 0) {
        return false;
    }

    uint64_t total_ship_cells = 0;
    for (const auto& ships_count : ship_counts_) {
        std::size_t ship_type = ships_count.first;
        uint64_t count = ships_count.second;
        uint64_t cells_for_type = ship_type * count;
        total_ship_cells += cells_for_type;
    }

    uint64_t total_cells = width_ * height_;
    if (total_ship_cells > total_cells) {
        return false;
    }

    return true;
}

void SlaveGame::PlaceInitialShips() {
    for (const auto& ships_count : ship_counts_) {
        std::size_t shipType = ships_count.first;
        uint64_t count = ships_count.second;
        for (uint64_t i = 0; i < count; ++i) {
            bool placed = false;
            for (std::size_t y = 0; y < height_ && !placed; ++y) {
                for (std::size_t x = 0; x < width_ && !placed; ++x) {
                    Ship ship(shipType, x, y, Orientation::kHorizontal);
                    if (field_.TryPlaceShip(ship)) {
                        placed = true;
                    }
                }
            }
        }
    }
}

// void SlaveGame::DebugPrintState() const {
//     DebugPrintRole();
//     DebugPrintGameState();
//     DebugPrintCounts();
//     std::cerr << "Strategy: " << current_strategy_ << "\n";
//     std::cerr << "Width: " << width_ << ", Height: " << height_ << "\n";
// }
//
// void SlaveGame::DebugPrintCounts() const {
//     std::cerr << "Counts:\n";
//     for (const auto& ships_count : ship_counts_) {
//         std::cerr << "Type " << ships_count.first << ": " << ships_count.second << "\n";
//     }
// }
//
// void SlaveGame::DebugPrintRole() const {
//     std::cerr << "Role: Slave\n";
// }
//
// void SlaveGame::DebugPrintGameState() const {
//     std::cerr << "is_game_created_: " << is_game_created_ << "\n";
//     std::cerr << "is_game_started_: " << is_game_started_ << "\n";
//     std::cerr << "is_game_stopped_: " << is_game_stopped_ << "\n";
//     std::cerr << "is_game_finished_: " << is_game_finished_ << "\n";
//     std::cerr << "are_we_winner_: " << are_we_winner_ << "\n";
//     std::cerr << "are_we_loser_: " << are_we_loser_ << "\n";
// }
