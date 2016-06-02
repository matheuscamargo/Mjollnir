#include "GameLogic.h"
#include <iostream>
#include <stdio.h>

namespace mjollnir { namespace vigridr {

GameLogic::GameLogic(const std::vector<int32_t> &playerIds) {
  const std::string kNoWinner = "-1";
  player1_ = playerIds[0];
  player2_ = playerIds[1];
  winner_ = kNoWinner;
  hasFinished_ = false;
  // TODO: implement initialization logic
}

bool GameLogic::update(Command command, int32_t playerId) {
  // TODO: implement update logic
  return true;
}

WorldModel GameLogic::getWorldModel() const {
  return worldModel_;
}

bool GameLogic::isFinished() const {
  return hasFinished_;
}
  
std::string GameLogic::getWinner() const {
  return winner_;
}

GameDescription GameLogic::getGameDescription(int32_t playerId) const {
  GameDescription gameDescription;
  // TODO: implement game description logic
  return gameDescription;
}

TotalWorldModel GameLogic::getTotalWorldModel() const {
  return twm_;
}

bool GameLogic::shouldPrintWorldModel(int32_t playerId){
  return true;
}

bool GameLogic::shouldIncrementCycle(int32_t playerId){
  return true;
}

size_t GameLogic::getNumberOfPlayers() const {
  return numberOfPlayers_;
}

GameResult GameLogic::createGameResult(std::string result, int32_t id) {
  return GameResult();
}

}}  // namespaces
