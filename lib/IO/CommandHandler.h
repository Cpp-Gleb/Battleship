#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "IGame.h"
#include "SlaveGame.h"
#include "MasterGame.h"

class CommandHandler {
public:
    CommandHandler(IGame*& game_ref);

    void Run();

private:
    IGame*& game_;
    bool is_running_ = true;

    void Dialoge(const std::string& line);
    std::vector<std::string> Split(const std::string& line) const;
};
