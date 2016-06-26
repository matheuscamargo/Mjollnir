#include "ClientLogic.h"

#include <iostream>
#include <vector>

#include "../thrifts/gen-cpp/Command_constants.h"

using ::mjollnir::vigridr::Move;
using ::mjollnir::vigridr::Command;
using ::mjollnir::vigridr::WorldModel;
using ::mjollnir::vigridr::GameInit;
using ::mjollnir::vigridr::GameResult;
using ::mjollnir::vigridr::PlayerColor;
using ::mjollnir::vigridr::_PlayerColor_VALUES_TO_NAMES;
using ::mjollnir::vigridr::g_Command_constants;

/* Helper functions, not mandatory for a solution */

Move make_move(int32_t src, int32_t dst) {
  Move mv;
  mv.src = src;
  mv.dst = dst;
  return mv;
}

std::ostream& operator<<(std::ostream& os,const  Move& m) {
    return os << m.src << "->" << m.dst;
}

std::ostream& operator<<(std::ostream& os, Command& c) {
    os << "(";
    for (Move& move : c.moves) {
      os << move.src << "->" << move.dst;
      os << ", ";
    }
    return os << ")";
}

template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> v) {
    os << "[";
    for (auto item : v) {
      os << item << ", ";
    }
    return os << "]";
}

/* Globals (used in this example) */

size_t me;
size_t other;
int32_t lastTurn;
Command lastCommand;
int32_t direction;
int32_t start;
int32_t end;

/*
 * This function is called at the beginning of the game.
 * You may do initialization here.
 *
 * Parameter:
 *     gameInit - contains a field named gameDescription, which itself contains a field named myColor.
 *                myColor is of type PlayerColor, which is an enum. PlayerColor has two fields: RED and WHITE.
 */
void init(const GameInit& gameInit) {
    std::cout << "C++ Backgammon Client" << std::endl;

    PlayerColor color = gameInit.gameDescription.myColor;
    std::cout << "PlayerColor: " << _PlayerColor_VALUES_TO_NAMES.at(color) << std::endl;

    me = static_cast<size_t>(color);
    lastTurn = -1;

    if (color == PlayerColor::RED) {
        other = static_cast<size_t>(PlayerColor::WHITE);
        direction = +1;
        start = 0;
        end = 23;
    }
    else {
        other = static_cast<size_t>(PlayerColor::RED);
        direction = -1;
        start = 23;
        end = 0;
    }
}

/*
 * This function is called once for every turn.
 * This specific example solution tries to move the checkers at the highest points.
 * However, it is not complete, so it sometimes sends invalid commands.
 *
 * Parameters:
 *     mwm   - an instance of the WorldModel class that contains the following fields:
 *            bar       - type Point. The number of checkers for each player in the bar.
 *            board     - vector of Point. Always contains 24 elements.
 *            borne_off - type Point. The number of checkers that each player has borne off.
 *            dice      - vector of int32_t. Always contains 2 elements.
 *            A Point is an alias for a vector of int32_t, with 2 elements, that represent
 *            the number of red and white checkers in that point, in that order. Hint: rembember that RED=0 and WHITE=1.
 *            Remember that a "point" is that triangle on the board where the checkers can be.
 *     turn - the index of the turn.
 *            If you receive twice the same number, don't worry, just ignore it.
 *
 * Returns:
 *     A Command instance - a Command contains a field called moves, which is a vector of Move.
 *                          A Move contains two fields of type int32_t, src and dst.
 *                          src and dst must be in the interval [0, 24).
 *                          Additionally, src can be FROM_BAR and dst can be BEAR_OFF.
 */
Command playTurn(const WorldModel& wm,
  const std::vector<Command>& moveList, int32_t turn) {

    std::cout << turn;
    std::cout << ": ";
    std::cout << wm.dice;
    std::cout << " ";

    Command command;

    size_t index = rand()%moveList.size();
    command = moveList[index];


    lastCommand = command;
      
    // Finally send command
    std::cout << " Command: " << command << std::endl;
    return command;
}

/*
 * This function is called at the end of the game.
 *
 * Parameters:
 *     result - an instance of the GameResult class, which contains two boolean fields, won and invalid.
 *              The invalid field is true if you lost because you sent an invalid command.
 */
void endOfGame(const GameResult& result) {
    std::cout << "End of game - " << (result.won ? "Won!" : "Lost...") << std::endl;
    if (result.invalid) {
        std::cout << "[WARNING] Invalid command" << std::endl;
    }
}
