#include "GameLogic.h"
#include <iostream>
#include <stdio.h>

namespace mjollnir { namespace vigridr {

GameLogic::GameLogic(int32_t playerId1, int32_t playerId2) {
  player1_ = playerId1;
  player2_ = playerId2;
  winner_ = "-1";
  hasFinished_ = false;
  std::vector<std::vector<Marker> > table {
      {Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED},
      {Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED},
      {Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED},
      {Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED},
      {Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED},
      {Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED},
      {Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED},
      {Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED},
      {Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED,
       Marker::UNMARKED,Marker::UNMARKED,Marker::UNMARKED}
  };
  worldModel_.table = table;
}

bool GameLogic::update(Command command, int32_t playerId) {
  if(!hasFinished_ && isValidMove(command.coordinate)) {
    if (playerId == player1_) {
      if (!isPassMove_(coordinate)) {
        //perform black's move
        bMove = step(BLACK, command.coordinate);
        setTableCoordinate_(command.coordinate, Marker::X);
      }
      else {
        bMove = -1;
      }
    }
    else if (playerId == player2_) {
      setTableCoordinate_(command.coordinate, Marker::O);
      hasFinished_ = checkVictory_(worldModel_, Marker::O , playerId);
    }
    //check for 2 passes in a row
    if( bMove == -1 && wMove == -1 ) {
      hasFinished_ = true;
    }
    return true;
  }
  return false;
}

GameDescription GameLogic::getGameDescription(int32_t playerId) const {
  GameDescription gameDescription;
  gameDescription.myType = (playerId == player1_) ? Marker::X : Marker::O;
  return gameDescription;
}

WorldModel GameLogic::getWorldModel() const {
  return worldModel_;
}

void GameLogic::setTableCoordinate(
    const Coordinate& coordinate, Marker marker) {
  setTableCoordinate_(coordinate, marker);
}

void GameLogic::setTableCoordinate_(
    const Coordinate& coordinate, Marker marker) {
  worldModel_.table[coordinate.x][coordinate.y] = marker;
}

bool GameLogic::isFinished() const {
  return hasFinished_;
}

void GameLogic::setHasFinished(bool value) {
  hasFinished_ = value;
}
  
std::string GameLogic::getWinner() const {
  return winner_;
}

void GameLogic::setWinner(std::string value) {
  winner_ = value;
}

bool GameLogic::randomPlay_(int32_t playerId) {
  for(auto& line : worldModel_.table)
    for(auto& element : line)
      if(element == Marker::UNMARKED) {
        if(playerId == player1_) {
          element = Marker::X;
          return true;
        }
        else if(playerId == player2_) {
          element = Marker::O;
          return true;
        }
      }
  return false;
}

bool GameLogic::checkLines_(const WorldModel& wm, Marker player) {
  for(const auto& line : wm.table) {
    for(size_t i = 0; i < boardSize_; ++i) {
      if(line[i] != player) break;
      if(i == boardSize_-1) return true;
    }
  }
  return false;
}

bool GameLogic::checkColumns_(const WorldModel& wm, Marker player) {
  for(size_t j = 0; j < boardSize_; ++j) {
    for(size_t i = 0; i < boardSize_; ++i) {
      if(wm.table[i][j] != player) break;
      if(i == boardSize_-1) return true;
    }
  }
  return false;
}

bool GameLogic::checkDiagonals_(const WorldModel& wm, Marker player) {
  for(size_t i = 0; i < boardSize_; ++i) {
    if(wm.table[i][boardSize_-1-i] != player) break;
    if(i == boardSize_-1) return true;
  }
  for(size_t i = 0; i < boardSize_; ++i) {
    if(wm.table[i][i] != player) break;
    if(i == boardSize_-1) return true;
  }
  return false;
}

bool GameLogic::checkVictory_(const WorldModel& wm, Marker player, int32_t playerId) {
  if(checkLines_(worldModel_, player) ||
     checkColumns_(worldModel_, player) ||
     checkDiagonals_(worldModel_, player) ) {
    winner_ = std::to_string(playerId);
    return true;
  }
  return false;
}

bool GameLogic::checkDraw_(const WorldModel& wm) {
  for(const auto& line : wm.table) {
    for(size_t i = 0; i < boardSize_; ++i) {
      if(line[i] == Marker::UNMARKED) return false;
    }
  }
  return true;
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

TotalWorldModel GameLogic::getTotalWorldModel() const {
  return twm_;
}

GameResult GameLogic::createGameResult(std::string result, int32_t id) {
  if (result == std::to_string(id)) {
    return GameResult::WON;
  }
  if (result == "-1") {
    return GameResult::TIED;
  }
  return GameResult::LOST;
}

bool GameLogic::isOnBoard_(const Coordinate& coordinate)
{
  // check that move is inside the board space
  if (coordinate.x >= 0 && coordinate.x < boardSize_ &&
      y >= 0 && y < boardSize_) {
    return true;
  }    
    return false;
}

bool GameLogic::isKo_(const Coordinate& coordinate)
{
  // check if move is equivalent to the previous game state
  if (coordinate.x == koPosX && coordinate.y == koPosY)
  {
    return true;
  }
  return false;
}

bool GameLogic::isPlayer_(
    const Coordinate& coordinate, Marker marker) {
  return worldModel_.table[coordinate.x][coordinate.y] == marker;
}

bool GameLogic::isValidMove_(const Coordinate& coordinate)
{
  // passing turn
  if (isPassMove_(coordinate))
    return true;
  if (isOnBoard_(coordinate) &&
      isPlayer_(coordinate, Marker::UNMARKED) &&
      !isKo_(coordinate)) {
    return true;
  }
  return false;
}

bool GameLogic::isPassMove_(const Coordinate& coordinate) {
  return coordinate.x == -1 && coordinate.y == -1;
}






}}  // namespaces
