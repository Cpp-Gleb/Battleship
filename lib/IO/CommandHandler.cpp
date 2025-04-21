#include "CommandHandler.h"
#include <cctype>

CommandHandler::CommandHandler(IGame*& game_ref)
    : game_(game_ref) {}

void CommandHandler::Run() {
    while (is_running_) {
        std::string line;
        if (!std::getline(std::cin, line)) {
            break;
        }
        Dialoge(line);
    }
}

std::vector<std::string> CommandHandler::Split(const std::string& line) const {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string tok;
    while (iss >> tok) {
        for (auto &ch : tok) {
            ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }
        tokens.push_back(tok);
    }
    return tokens;
}

void CommandHandler::Dialoge(const std::string& line) {
    auto tokens = Split(line);
    if (tokens.empty()) {
        return;
    }

    std::string comand = tokens[0];

    if (comand == "ping") {
        std::cout << "pong\n";
    } else if (comand == "exit") {
        std::cout << "ok\n";
        is_running_ = false;
    } else if (comand == "create") {
        if (tokens.size() != 2) {
            std::cout << "failed\n";
            return;
        }
        std::string role = tokens[1];
        PlayerRole player_role;
        if (role == "master") {
            player_role = PlayerRole::Master;
        } else if (role == "slave") {
            player_role = PlayerRole::Slave;
        } else {
            std::cout << "failed\n";
            return;
        }
        if (!game_) {
            if (player_role == PlayerRole::Master) {
                game_ = new MasterGame();
            } else {
                game_ = new SlaveGame();
            }
        }
        if (game_->TryCreate(player_role)) {
            std::cout << "ok\n";
        } else {
            std::cout << "failed\n";
        }
    } else if (comand == "start") {
        if (!game_) { std::cout << "failed\n"; return; }
        if (game_->TryStart()) {
            std::cout << "ok\n";
        } else {
            std::cout << "failed\n";
        }
    } else if (comand == "stop") {
        if (!game_) { std::cout << "failed\n"; return; }
        if (game_->TryStop()) {
            std::cout << "ok\n";
        } else {
            std::cout << "failed\n";
        }
    } else if (comand == "set") {
        if (!game_) { std::cout << "failed\n"; return; }
        if (tokens.size() < 3) {
            std::cout << "failed\n";
            return;
        }
        std::string parametr = tokens[1];
        if (parametr == "width") {
            if (tokens.size() != 3) {std::cout<<"failed\n";return;}
            uint64_t width = std::stoull(tokens[2]);
            if (game_->TrySetWidth(width)) {
                std::cout << "ok\n";
            } else {
                std::cout << "failed\n";
            }
        } else if (parametr == "height") {
            if (tokens.size() != 3) {std::cout<<"failed\n";return;}
            uint64_t height = std::stoull(tokens[2]);
            if (game_->TrySetHeight(height)) {
                std::cout << "ok\n";
            } else {
                std::cout << "failed\n";
            }
        } else if (parametr == "count") {
            if (tokens.size() != 4) {std::cout<<"failed\n";return;}
            std::size_t type = static_cast<std::size_t>(std::stoull(tokens[2]));
            uint64_t count = std::stoull(tokens[3]);
            if (game_->TrySetCount(type, count)) {
                std::cout << "ok\n";
            } else {
                std::cout << "failed\n";
            }
        } else if (parametr == "strategy") {
            if (tokens.size() != 3) {std::cout<<"failed\n";return;}
            std::string strategy = tokens[2];
            if (game_->TrySetStrategy(strategy)) {
                std::cout << "ok\n";
            } else {
                std::cout << "failed\n";
            }
        } else if (parametr == "result") {
            if (tokens.size() != 3) {std::cout<<"failed\n";return;}
            std::string result = tokens[2];
            if (game_->TrySetResult(result)) {
                std::cout << "ok\n";
            } else {
                std::cout << "failed\n";
            }
        } else {
            std::cout << "failed\n";
        }
    } else if (comand == "get") {
        if (!game_) {std::cout<<"failed\n";return;}
        if (tokens.size() < 2) {std::cout<<"failed\n";return;}
        std::string parametr = tokens[1];
        if (parametr == "width") {
            if (tokens.size() != 2) {std::cout<<"failed\n";return;}
            uint64_t width = game_->GetWidth();
            std::cout << width << "\n";
        } else if (parametr == "height") {
            if (tokens.size() != 2) {std::cout<<"failed\n";return;}
            uint64_t height = game_->GetHeight();
            std::cout << height << "\n";
        } else if (parametr == "count") {
            if (tokens.size() != 3) {std::cout<<"failed\n";return;}
            std::size_t type = static_cast<std::size_t>(std::stoull(tokens[2]));
            uint64_t count = game_->GetCount(type);
            std::cout << count << "\n";
        } else {
            std::cout << "failed\n";
        }
    } else if (comand == "shot") {
        if (!game_) {std::cout<<"miss\n";return;}
        if (tokens.size() == 3) {
            uint64_t x = std::stoull(tokens[1]);
            uint64_t y = std::stoull(tokens[2]);
            std::string result = game_->IncomingShot(x,y);
            std::cout << result << "\n";
        } else if (tokens.size() == 1) {
            std::string coord = game_->GetNextShot();
            std::cout << coord << "\n";
        } else if (tokens.size() == 2) {
            std::cout << "miss\n";
        } else {
            std::cout << "miss\n";
        }
    } else if (comand == "finished") {
        if (!game_) {std::cout<<"no\n";return;}
        std::cout << game_->Finished() << "\n";
    } else if (comand == "win") {
        if (!game_) {std::cout<<"no\n";return;}
        std::cout << game_->Win() << "\n";
    } else if (comand == "lose") {
        if (!game_) {std::cout<<"no\n";return;}
        std::cout << game_->Lose() << "\n";
    } else if (comand == "dump") {
        if (!game_) {std::cout<<"failed\n";return;}
        if (tokens.size() != 2) {std::cout<<"failed\n";return;}
        if (game_->TryDump(tokens[1].c_str())) {
            std::cout << "ok\n";
        } else {
            std::cout << "failed\n";
        }
    } else if (comand == "load") {
        if (!game_) {
            game_ = new SlaveGame();
            game_->TryCreate(PlayerRole::Slave);
        }
        if (tokens.size() != 2) {std::cout<<"failed\n";return;}
        if (game_->TryLoad(tokens[1].c_str())) {
            std::cout << "ok\n";
        } else {
            std::cout << "failed\n";
        }
    } else {
        std::cout << "failed\n";
    }
}
