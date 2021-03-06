#include "ClientLogic.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>

using ::mjollnir::vigridr::Command;
using ::mjollnir::vigridr::Coordinate;
using ::mjollnir::vigridr::WorldModel;
using ::mjollnir::vigridr::GameInit;
using ::mjollnir::vigridr::GameResult;
using ::mjollnir::vigridr::_GameResult_VALUES_TO_NAMES;
using ::mjollnir::vigridr::Marker;

/*
 * A helper function to print a Coordinate on screen.
 */
// std::ostream& operator<<(std::ostream& os, Coordinate c) {
//     return os << "Coordinate(" << c.x << ", " << c.y << ")";
// }

/*
 * This function is called at the beginning of the game.
 * You may do initialization here.
 *
 * Parameter:
 *     gameInit - contains a field named gameDescription, which itself contains a field named myType.
 *                myType is of type Marker, which is an enum. Marker has three fields: UNMARKED, X and O.
 */
void init(const GameInit& gameInit) {
  srand(time(NULL));

  std::cout << "C++ Client" << std::endl;
  if(gameInit.gameDescription.myType == Marker::X) {
    std::cout << "PlayerType: X (BLACK)" << std::endl;
  }
  else{
    std::cout << "PlayerType: O (WHITE)" << std::endl;
  }
}

/*
 * This function is called once for every turn.
 * This specific example solution returns a random valid position.
 *
 * Parameters:
 *     wm   - an instance of the WorldModel class that contains a field called table which is a vector of vectors of Markers.
 *     turn - the index of the turn.
 *            If you receive twice the same number, don't worry, just ignore it.
 *
 * Returns:
 *     A Command instance - a Command contains a field called coordinate of class Coordinate.
 *                          A Coordinate contains two fields of type int32_t, x and y.
 */
Command playTurn(const WorldModel& wm,
  const std::vector<Command>& moveList, int32_t turn) {
  Command command;

  size_t index = rand()%moveList.size();
  command = moveList[index];

  std::cout << turn << ": " << "Coordinate(" << command.coordinate.x << ", " << command.coordinate.y << ")" << std::endl;
  return command;
}

/*
 * This function is called at the end of the game.
 *
 * Parameters:
 *     result - an instance of the GameResult enum, which can be GameResult::WON, GameResult::TIED or GameResult::LOST.
 */
void endOfGame(const GameResult& result) {
  std::cout << "End of game - " << _GameResult_VALUES_TO_NAMES.at(result) << std::endl;
}
