#include "ClientLogic.h"

#include <iostream>

using namespace ::mjollnir::vigridr;

/*
 * This function is called at the beginning of the game.
 * You may do initialization here.
 *
 * Parameter:
 *     gameInit - contains a field named gameDescription, which itself contains fields myIndex and 'field'.
 *                myIndex is an int32_t and represents the index of the world model table of your snake.
 *                'field' is an instance of the class Field, and contains the fields width and height, of type int32_t.
 *                It also contains a field called gameInfo with another field, worldModel, of type WorldModel, described below.
 */
void init(const GameInit& gameInit) {
    std::cout << "C++ Client" << std::endl;
    srand(time(NULL));
}

/*
 * This function is called once for every turn.
 * It returns an empty command. For a more interesting example, see the python sample solution.
 *
 * Parameters:
 *     wm   - an instance of the WorldModel class that contains a field called players which is a vector of Players.
 *            A Player has a field named body which is a vector of Coordinates,
 *            and which represent the coordinates of the body parts of the snake, in order.
 *            A Coordinate has two fields, x and y, of type int32_t.
 *     turn - the index of the turn.
 *            If you receive twice the same number, don't worry, just ignore it.
 *
 * Returns:
 *     A Command instance - a Command contains a field called direction of type enum Direction.
 *                          Direction values: RIGHT, UP, LEFT, DOWN.
 */
Command playTurn(const WorldModel& wm,
  const std::vector<Command>& moveList, int32_t turn) {
  Command command;

  size_t index = rand()%moveList.size();
  command = moveList[index];
  
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
