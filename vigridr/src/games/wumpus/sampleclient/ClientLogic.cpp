#include "ClientLogic.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>

using ::mjollnir::vigridr::Command;
using ::mjollnir::vigridr::WorldModel;
using ::mjollnir::vigridr::GameInit;
using ::mjollnir::vigridr::GameResult;
using ::mjollnir::vigridr::Action;

/*
 * This function is called at the beginning of the game.
 * You may do initialization here.
 *
 * Parameter:
 *     gameInit - not used for Wumpus
 */
void init(const GameInit& gameInit) {
  srand(time(NULL));
  std::cout << "C++ Client" << std::endl;
}

/*
 * This function is called once for every turn.
 * This specific example solution returns a random action.
 *
 * Parameters:
 *     wm   - an instance of the WorldModel class that contains a field called sensors of class Sensors.
 *            Sensors contains the boolean fields: breeze, stench, glitter, bump and scream.
 *     turn - the index of the turn.
 *            If you receive twice the same number, don't worry, just ignore it.
 *
 * Returns:
 *     A Command instance - a Command contains a field called action of enum Action.
 *                          Action fields: FORWARD, TURNRIGHT, TURNLEFT, STAY, SHOOT, GRAB and CLIMB.
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
 *     result - an instance of the GameResult class, that has only one int32_t field, score.
 */
void endOfGame(const GameResult& result) {
  std::cout << "End of game - Score: " << result.score << std::endl;
}
