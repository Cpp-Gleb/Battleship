#include "MasterGame.h"

MasterGame::MasterGame()
    : role_(PlayerRole::Master),
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

MasterGame::~MasterGame() {
    if (strategy_) {
        delete strategy_;
    }
    delete zone_manager_;
    delete pattern_manager_;
    delete hunting_manager_;
    delete stats_;
}

bool MasterGame::TryCreate(PlayerRole role) {
    if (is_game_started_ || is_game_created_) {
        return false;
    }
    if (role != PlayerRole::Master) {
        return false;
    }
    role_ = role;
    is_game_created_ = true;
    return true;
}

bool MasterGame::TrySetWidth(uint64_t width) {
    if (!is_game_created_ || is_game_started_) {
        return false;
    }
    if (width == 0) {
        return false;
    }
    width_ = width;
    return true;
}

bool MasterGame::TrySetHeight(uint64_t height) {
    if (!is_game_created_ || is_game_started_) {
        return false;
    }

    if (height == 0) {
        return false;
    }

    height_ = height;
    return true;
}

uint64_t MasterGame::GetWidth() const {
    return width_;
}

uint64_t MasterGame::GetHeight() const {
    return height_;
}

bool MasterGame::TrySetCount(std::size_t ship_type, uint64_t count) {
    if (!is_game_created_ || is_game_started_) {
        return false;
    }

    if (ship_type < 1 || ship_type > 4) {
        return false;
    }

    ship_counts_[ship_type] = count;
    return true;
}

uint64_t MasterGame::GetCount(std::size_t ship_type) const {
    auto it = ship_counts_.find(ship_type);
    if (it != ship_counts_.end()) {
        return it->second;
    }

    return 0;
}

bool MasterGame::TrySetStrategy(const std::string& strat) {
    if (!is_game_created_) {
        return false;
    }

    if (strat != "ordered" && strat != "custom") {
        return false;
    }

    current_strategy_ = strat;
    return true;
}

bool MasterGame::TryStart() {
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

bool MasterGame::TryStop() {
    if (!is_game_started_) {
        return false;
    }
    is_game_stopped_ = true;
    return true;
}

std::string MasterGame::IncomingShot(uint64_t x, uint64_t y) {
    if (!is_game_started_ || is_game_finished_) {
        return "miss";
    }
    ShotResult res = field_.Shot(x, y);
    switch (res) {
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

std::string MasterGame::GetNextShot() {
    if (!is_game_started_ || is_game_finished_) {
        return "0 0";
    }
    auto shot = strategy_->GetNextShot();
    return std::to_string(shot.first) + " " + std::to_string(shot.second);
}

bool MasterGame::TrySetResult(const std::string& res) {
    if (!is_game_started_ || is_game_finished_) {
        return false;
    }

    ShotResult shot_result;
    if (res == "miss") {
        shot_result = ShotResult::kMiss;
    } else if (res == "hit") {
        shot_result = ShotResult::kHit;
    } else if (res == "kill") {
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

std::string MasterGame::Finished() const {
    return is_game_finished_ ? "yes" : "no";
}

std::string MasterGame::Win() const {
    if (!is_game_finished_) {
        return "no";
    }
    return are_we_winner_ ? "yes" : "no";
}

std::string MasterGame::Lose() const {
    if (!is_game_finished_) {
        return "no";
    }
    return are_we_loser_ ? "yes" : "no";
}

bool MasterGame::TryDump(const char* path) {
    return field_.TryDump(path);
}

bool MasterGame::TryLoad(const char* path) {
    if (!field_.TryLoad(path)) {
        return false;
    }

    for (const auto& ship : field_.GetAllShips()) {
        if (!field_.TryPlaceShip(ship)) {
            return false;
        }
    }

    return true;
}

bool MasterGame::CanStartGame() const {
    if (width_ == 0 || height_ == 0) {
        return false;
    }
    bool is_any_ship_alive = false;
    for (const auto& ship_count : ship_counts_) {
        if (ship_count.second > 0) {
            is_any_ship_alive = true;
            break;
        }
    }
    return is_any_ship_alive;
}

void MasterGame::InitializeStrategy() {
    if (current_strategy_ == "ordered") {
        strategy_ = new OrderedStrategy(static_cast<std::size_t>(width_), static_cast<std::size_t>(height_));
    } else {
        strategy_ = new CustomStrategy(GetCount(4), GetCount(3), GetCount(2), GetCount(1), zone_manager_, pattern_manager_, hunting_manager_, stats_);
    }
}

bool MasterGame::ValidateFieldAndShips() const {
    if (width_ == 0 || height_ == 0) {
        return false;
    }

    uint64_t total_ship_cells = 0;
    for (const auto& ship_count : ship_counts_) {
        std::size_t ship_type = ship_count.first;
        uint64_t count = ship_count.second;
        uint64_t cells_for_type = ship_type * count;
        total_ship_cells += cells_for_type;
    }

    uint64_t total_cells = width_ * height_;
    if (total_ship_cells > total_cells) {
        std::cerr << "count of ship more than field";
        return false;
    }

    return true;
}

void MasterGame::PlaceInitialShips() {
    for (const auto& ship_count : ship_counts_) {
        std::size_t ship_type = ship_count.first;
        uint64_t count = ship_count.second;
        for (uint64_t i = 0; i < count; ++i) {
            bool placed = false;
            for (std::size_t y = 0; y < height_ && !placed; ++y) {
                for (std::size_t x = 0; x < width_ && !placed; ++x) {
                    Ship ship(ship_type, x, y, Orientation::kHorizontal);
                    if (field_.TryPlaceShip(ship)) {
                        placed = true;
                    }
                }
            }
        }
    }
}

// void MasterGame::DebugPrintState() const {
//     DebugPrintRole();
//     DebugPrintGameState();
//     DebugPrintCounts();
//     std::cerr << "Strategy: " << current_strategy_ << "\n";
//     std::cerr << "Width: " << width_ << ", Height: " << height_ << "\n";
// }
//
// void MasterGame::DebugPrintCounts() const {
//     std::cerr << "Counts:\n";
//     for (const auto& ship_count : ship_counts_) {
//         std::cerr << "Type " << ship_count.first << ": " << ship_count.second << "\n";
//     }
// }
//
// void MasterGame::DebugPrintRole() const {
//     std::cerr << "Role: Master\n";
// }
//
// void MasterGame::DebugPrintGameState() const {
//     std::cerr << "is_game_created_: " << is_game_created_ << "\n";
//     std::cerr << "is_game_started_: " << is_game_started_ << "\n";
//     std::cerr << "is_game_stopped_: " << is_game_stopped_ << "\n";
//     std::cerr << "is_game_finished_: " << is_game_finished_ << "\n";
//     std::cerr << "are_we_winner_: " << are_we_winner_ << "\n";
//     std::cerr << "are_we_loser_: " << are_we_loser_ << "\n";
// }
