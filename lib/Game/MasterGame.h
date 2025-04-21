#pragma once

#include "IGame.h"
#include "Field.h"
#include "IStrategy.h"
#include "OrderedStrategy.h"
#include "CustomStrategy/CustomStrategy.h"
#include "CustomStrategy/Zone/ZoneManager.h"
#include "CustomStrategy/Hunting/HuntingManager.h"
#include "CustomStrategy/Pattern/PatternManager.h"
#include "CustomStrategy/Statistics/Statistics.h"

#include <map>
#include <string>

class MasterGame : public IGame {
public:
    MasterGame();
    ~MasterGame();

    bool TryCreate(PlayerRole role) override;
    bool TrySetWidth(uint64_t w) override;
    bool TrySetHeight(uint64_t h) override;
    uint64_t GetWidth() const override;
    uint64_t GetHeight() const override;
    bool TrySetCount(std::size_t shipType, uint64_t count) override;
    uint64_t GetCount(std::size_t shipType) const override;
    bool TrySetStrategy(const std::string& strat) override;
    bool TryStart() override;
    bool TryStop() override;
    std::string IncomingShot(uint64_t x, uint64_t y) override;
    std::string GetNextShot() override;
    bool TrySetResult(const std::string& res) override;
    std::string Finished() const override;
    std::string Win() const override;
    std::string Lose() const override;
    bool TryDump(const char* path) override;
    bool TryLoad(const char* path) override;

    // void DebugPrintState() const override;

private:
    PlayerRole role_;
    bool is_game_created_;
    bool is_game_started_;
    bool is_game_stopped_;
    bool is_game_finished_;
    bool are_we_winner_;
    bool are_we_loser_;

    uint64_t width_;
    uint64_t height_;
    std::map<std::size_t, uint64_t> ship_counts_;

    IStrategy* strategy_;
    std::string current_strategy_;

    Field field_;

    IZoneManager* zone_manager_;
    IPatternManager* pattern_manager_;
    IHuntingManager* hunting_manager_;
    Statistics* stats_;

    bool CanStartGame() const;
    void InitializeStrategy();
    bool ValidateFieldAndShips() const;
    void PlaceInitialShips();

    // void DebugPrintCounts() const;
    // void DebugPrintRole() const;
    // void DebugPrintGameState() const;
};

