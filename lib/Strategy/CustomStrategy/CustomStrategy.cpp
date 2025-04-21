#include "CustomStrategy.h"

CustomStrategy::CustomStrategy(uint64_t battleships, uint64_t cruisers, uint64_t destroyers, uint64_t submarines, IZoneManager* zoneManager, IPatternManager* patternManager, IHuntingManager* huntingManager, Statistics* stats)
    :
      battleships_(battleships),
      cruisers_(cruisers),
      destroyers_(destroyers),
      submarines_(submarines),
      game_phase_(Phase::Searching),
      zone_manager_(zoneManager),
      pattern_manager_(patternManager),
      hunting_manager_(huntingManager),
      stats_(stats)
{
    for (std::size_t y = 0; y < KLOCAL_HEIGHT; y++) {
        for (std::size_t x = 0; x < KLOCAL_WIDTH; x++) {
            enemy_map_[y][x] = EnemyCellState::Unknown;
        }
    }
    std::srand((unsigned)std::time(nullptr));
    UpdateCurrentPatternType();
}
std::pair<std::size_t, std::size_t> CustomStrategy::GetNextShot() {
    if (game_phase_ == Phase::Hunting && hunting_manager_->HasTarget()) {
        auto shot = hunting_manager_->GetHuntingShot(enemy_map_);
        if (shot.has_value()) {
            return shot.value();
        }
        game_phase_ = Phase::Searching;
    }

    UpdateCurrentPatternType();
    auto patternType = pattern_manager_->GetCurrentPatternType();
    return pattern_manager_->GetNextShot(enemy_map_, *zone_manager_, patternType);
}

void CustomStrategy::UpdateStrategy(std::pair<std::size_t,std::size_t> shot, ShotResult result) {
    auto [x,y] = shot;
    switch (result) {
        case ShotResult::kMiss:
            enemy_map_[y][x] = EnemyCellState::Miss;
            stats_->RecordMiss();
            if (game_phase_ == Phase::Hunting) {
                hunting_manager_->OnMiss(shot, enemy_map_);
            }
            break;
        case ShotResult::kHit:
            enemy_map_[y][x] = EnemyCellState::Hit;
            stats_->RecordHit();
            if (game_phase_ == Phase::Searching) {
                game_phase_ = Phase::Hunting;
                hunting_manager_->StartHunt(shot, enemy_map_);
            } else {
                hunting_manager_->AddHit(shot);
            }
            break;
        case ShotResult::kKill:
            enemy_map_[y][x] = EnemyCellState::Hit;
            stats_->RecordHit();
            hunting_manager_->AddHit(shot);
            hunting_manager_->MarkNoShipAround(enemy_map_);
            IdentifySunkShip(hunting_manager_->GetFoundShipSize());
            hunting_manager_->Reset();
            game_phase_ = Phase::Searching;
            break;
    }

    if (zone_manager_->NeedZoneSwitch(game_phase_, *stats_)) {
        zone_manager_->SwitchToNextZone();
        pattern_manager_->ResetPatternIndex();
    }
}

void CustomStrategy::UpdateCurrentPatternType() {
    if (battleships_ > 0) {
        pattern_manager_->SetPatternForTarget("battleship");
    } else if (cruisers_ > 0) {
        pattern_manager_->SetPatternForTarget("cruiser");
    } else if (destroyers_ > 0) {
        pattern_manager_->SetPatternForTarget("destroyer");
    } else if (submarines_ > 0) {
        pattern_manager_->SetPatternForTarget("submarine");
    }
}

void CustomStrategy::IdentifySunkShip(std::size_t size) {
    switch(size) {
        case 4:
            if (battleships_ > 0) {
                battleships_--;
            }
            break;
        case 3:
            if (cruisers_ > 0) {
                cruisers_--;
            }
            break;
        case 2:
            if (destroyers_ > 0) {
                destroyers_--;
            }
            break;
        case 1:
            if (submarines_ > 0) {
                submarines_--;
            }
            break;
        default:
            break;
    }
}