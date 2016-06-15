#include "GameLogic.h"
#include <iostream>
#include <stdio.h>

namespace mjollnir { namespace vigridr {

GameLogic::GameLogic(const std::vector<int32_t> &playerIds) {
  player1_ = playerIds[0];
  player2_ = playerIds[1];
  winner_ = "-1";
  hasFinished_ = false;
  std::vector<std::vector<Piece> > board;
  for (size_t i = 0; i < boardSize_; i++) {
    std::vector<Piece> line;    
    for (size_t i = 0; i < boardSize_; i++) {
      Piece newPiece;
      newPiece.type = Type::EMPTY;
      line.push_back(newPiece);
    }
    board.push_back(line);
  }

  board[0][0] = createPiece(Type::TOWER, PlayerColor::BLACK);
  board[0][1] = createPiece(Type::HORSE, PlayerColor::BLACK);
  board[0][2] = createPiece(Type::BISHOP, PlayerColor::BLACK);
  board[0][3] = createPiece(Type::QUEEN, PlayerColor::BLACK);
  board[0][4] = createPiece(Type::KING, PlayerColor::BLACK);
  board[0][5] = createPiece(Type::BISHOP, PlayerColor::BLACK);
  board[0][6] = createPiece(Type::HORSE, PlayerColor::BLACK);
  board[0][7] = createPiece(Type::TOWER, PlayerColor::BLACK);
  board[7][0] = createPiece(Type::TOWER, PlayerColor::WHITE);
  board[7][1] = createPiece(Type::HORSE, PlayerColor::WHITE);
  board[7][2] = createPiece(Type::BISHOP, PlayerColor::WHITE);
  board[7][3] = createPiece(Type::QUEEN, PlayerColor::WHITE);
  board[7][4] = createPiece(Type::KING, PlayerColor::WHITE);
  board[7][5] = createPiece(Type::BISHOP, PlayerColor::WHITE);
  board[7][6] = createPiece(Type::HORSE, PlayerColor::WHITE);
  board[7][7] = createPiece(Type::TOWER, PlayerColor::WHITE);

  for (int i = 0; i < 8; i++) {
    board[1][i] = createPiece(Type::PAWN, PlayerColor::BLACK);
    board[6][i] = createPiece(Type::PAWN, PlayerColor::WHITE);
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
  
  Piece emptyPiece;
  emptyPiece.type = Type::EMPTY;
  worldModel_.board[command.coordTo.x][command.coordTo.y] =
    worldModel_.board[command.coordFrom.x][command.coordFrom.y];
  worldModel_.board[command.coordFrom.x][command.coordFrom.y] =
    emptyPiece;
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

Piece GameLogic::createPiece(Type type, PlayerColor owner) {
  Piece newPiece;
  newPiece.type = type;
  newPiece.owner = owner;
  return newPiece;
}

}}  // namespaces
