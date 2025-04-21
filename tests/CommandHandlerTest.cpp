#include <gtest/gtest.h>
#include "IO/CommandHandler.h"
#include "Game/MasterGame.h"
#include "Game/SlaveGame.h"
#include <sstream>

class CommandHandlerTest : public ::testing::Test {};

TEST_F(CommandHandlerTest, PingExit) {
    IGame* game = nullptr;
    CommandHandler handler(game);

    std::istringstream input("ping\nexit\n");
    std::ostringstream output;
    std::streambuf* cinBuf = std::cin.rdbuf(input.rdbuf());
    std::streambuf* coutBuf = std::cout.rdbuf(output.rdbuf());

    handler.Run();

    std::cin.rdbuf(cinBuf);
    std::cout.rdbuf(coutBuf);

    std::string outStr = output.str();
    EXPECT_NE(outStr.find("pong"), std::string::npos);
    EXPECT_NE(outStr.find("ok"), std::string::npos);

    if (game) delete game;
}

TEST_F(CommandHandlerTest, CreateMasterAndSetParameters) {
    IGame* game = nullptr;
    CommandHandler handler(game);

    std::istringstream input("create master\nset width 10\nset height 10\nset count 1 2\nstart\nexit\n");
    std::ostringstream output;
    std::streambuf* cinBuf = std::cin.rdbuf(input.rdbuf());
    std::streambuf* coutBuf = std::cout.rdbuf(output.rdbuf());

    handler.Run();

    std::cin.rdbuf(cinBuf);
    std::cout.rdbuf(coutBuf);

    std::string outStr = output.str();
    size_t pos = outStr.find("ok");
    EXPECT_NE(pos, std::string::npos);
    pos = outStr.find("ok", pos + 1);
    EXPECT_NE(pos, std::string::npos);
    pos = outStr.find("ok", pos + 1);
    EXPECT_NE(pos, std::string::npos);
    pos = outStr.find("ok", pos + 1);
    EXPECT_NE(pos, std::string::npos);

    ASSERT_NE(game, nullptr);
    EXPECT_EQ(game->GetWidth(), 10ULL);
    EXPECT_EQ(game->GetHeight(), 10ULL);
    EXPECT_EQ(game->GetCount(1), 2ULL);
}
