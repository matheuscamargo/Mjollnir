#include "GameLogic.h"
#include <iostream>
#include <stdio.h>

namespace mjollnir { namespace vigridr {

GameLogic::GameLogic(int32_t playerId1, int32_t playerId2) {
  player1_ = playerId1;
  player2_ = playerId2;
  winner_ = "-1";
  hasFinished_ = false;
  std::vector<std::vector<Piece> > board;
  for (size_t i = 0; i < boardSize_; i++) {
    std::vector<Piece> line;    
    for (size_t i = 0; i < boardSize_; i++) {
      line.push_back(Piece::EMPTY);
    }
    board.push_back(line);
  }
  board[0][0] = Piece::TOWER;
  board[0][1] = Piece::HORSE;
  board[0][2] = Piece::BISHOP;
  board[0][3] = Piece::KING;
  board[0][4] = Piece::QUEEN;
  board[0][5] = Piece::BISHOP;
  board[0][6] = Piece::HORSE;
  board[0][7] = Piece::TOWER;
  board[7][0] = Piece::TOWER;
  board[7][1] = Piece::HORSE;
  board[7][2] = Piece::BISHOP;
  board[7][3] = Piece::QUEEN;
  board[7][4] = Piece::KING;
  board[7][5] = Piece::BISHOP;
  board[7][6] = Piece::HORSE;
  board[7][7] = Piece::TOWER;
  for (int i = 0; i < 8; i++) {
    board[1][i] = Piece::PAWN;
    board[6][i] = Piece::PAWN;
  }
  worldModel_.board = board;
}

bool GameLogic::update(Command command, int32_t playerId) {
  if(tempGameCounter-- > 0) {

    movePiece(command);

    return true;
  }
  return false;
}

GameDescription GameLogic::getGameDescription(int32_t playerId) const {
  GameDescription gameDescription;
  gameDescription.myColor = (playerId == player1_) ?
    PlayerColor::BLACK : PlayerColor::WHITE;
  return gameDescription;
}

void GameLogic::movePiece(Command command) {
  worldModel_.board[command.coordTo.x][command.coordTo.y] =
    worldModel_.board[command.coordFrom.x][command.coordFrom.y];
  worldModel_.board[command.coordFrom.x][command.coordFrom.y] =
    Piece::EMPTY;
}

WorldModel GameLogic::getWorldModel() const {
  return worldModel_;
}

void GameLogic::setTableCoordinate(
    const Coordinate& coordinate, Piece piece) {
  setTableCoordinate_(coordinate, piece);
}

void GameLogic::setTableCoordinate_(
    const Coordinate& coordinate, Piece piece) {
  worldModel_.board[coordinate.x][coordinate.y] = piece;
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

}}  // namespaces
