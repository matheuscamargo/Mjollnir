#include "GameLogic.h"
#include <iostream>
#include <stdio.h>
#include <fstream>

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
  moveList_ = getAllValidMovesOfPlayer(PlayerColor::WHITE);
}

void GameLogic::setFirstPlayer(int32_t playerId) {
  whitePlayerId_ = playerId;
}

bool GameLogic::update(Command command, int32_t playerId) {
  bool isValidCommand = validCommand(command, playerId);

  if(isValidCommand) {
    setRockVars(command, playerId);
    setEnPassantVars(command, playerId);
    movePiece(command, playerId);
    if (playerId == whitePlayerId_) {
      moveList_ = getAllValidMovesOfPlayer(PlayerColor::BLACK);
    }
    else {      
      moveList_ = getAllValidMovesOfPlayer(PlayerColor::WHITE);
    }
    return true;
  }
  return false;
}

void GameLogic::setEnPassantVars(Command command, int32_t playerId){
  if( worldModel_.board[command.coordFrom.x][command.coordFrom.y].type == Type::PAWN ){
    if( whitePlayerId_ == playerId ){
      if( command.coordFrom.x == 6 && command.coordTo.x == 4 ){
        enPassant_.playerColor = PlayerColor::WHITE;
        enPassant_.lastMoveWasAPawnDoubleForward = true;
        enPassant_.pawnX = command.coordTo.x;
        enPassant_.pawnY = command.coordTo.y;
        return;
      }
      else {
      }
    } 
    else { // black player move
      if( command.coordFrom.x == 1 && command.coordTo.x == 3 ){
        enPassant_.playerColor = PlayerColor::BLACK;
        enPassant_.lastMoveWasAPawnDoubleForward = true;
        enPassant_.pawnX = command.coordTo.x;
        enPassant_.pawnY = command.coordTo.y;
        return;
      }
    }
  }
  enPassant_.lastMoveWasAPawnDoubleForward = false;
}

bool GameLogic::validCommand(Command command, int32_t playerId) {
  bool rockTried = false;
  if(command.smallRock || command.bigRock){
    rockTried = true;
  }

  validEnPassant_ = validEnPassant(command, playerId);

  if( rockTried ){
    return validRock(command, playerId);
  }
  else{
    return  validEnPassant_
            || (  validFrom(command, playerId)
                  && validMove(command, playerId)
                  && !playerInCheckAfterCommand(command, playerId) );
  }
}

bool GameLogic::validEnPassant(Command command, int32_t playerId){
  if( !validFrom(command, playerId) )
    return false;

  if( playerId == whitePlayerId_ ){
    return  enPassant_.lastMoveWasAPawnDoubleForward
            && command.coordFrom.x == enPassant_.pawnX
            && (command.coordFrom.y == enPassant_.pawnY+1 || command.coordFrom.y == enPassant_.pawnY-1 )
            && command.coordTo.x == 2 && command.coordTo.y == enPassant_.pawnY; 
  }
  else { // black player move
    return  enPassant_.lastMoveWasAPawnDoubleForward
            && command.coordFrom.x == enPassant_.pawnX
            && (command.coordFrom.y == enPassant_.pawnY+1 || command.coordFrom.y == enPassant_.pawnY-1 )
            && command.coordTo.x == 5 && command.coordTo.y == enPassant_.pawnY;
  }
}

bool GameLogic::validRock(Command command, int32_t playerId){
  bool validRock = false;

  if( playerId == whitePlayerId_ ){
    if( command.smallRock ){
      validRock = !rock_.whiteKingMoved && !rock_.whiteRightTowerMoved
                  && noHorizontalPiecesBetween(8-1, 4, 8-1)
                  && !playerInCheck(playerId) && !playerInCheckAfterCommand(command, playerId);
    }
    else if( command.bigRock ){
      validRock = !rock_.whiteKingMoved && !rock_.whiteLeftTowerMoved
                  && noHorizontalPiecesBetween(8-1, 0, 4)
                  && !playerInCheck(playerId) && !playerInCheckAfterCommand(command, playerId);
    }
  }
  else { // black player rock
    if( command.smallRock ){
      validRock = !rock_.blackKingMoved && !rock_.blackRightTowerMoved
                  && noHorizontalPiecesBetween(0, 4, 8-1)
                  && !playerInCheck(playerId) && !playerInCheckAfterCommand(command, playerId);
    }
    else if( command.bigRock ){
      validRock = !rock_.blackKingMoved && !rock_.blackLeftTowerMoved
                  && noHorizontalPiecesBetween(0, 0, 4)
                  && !playerInCheck(playerId) && !playerInCheckAfterCommand(command, playerId);
    }    
  }

  return validRock;
}

// y0 < y1
bool GameLogic::noHorizontalPiecesBetween(int32_t x0, int32_t y0, int32_t y1){
  for( int32_t y = y0+1; y < y1; y++ ){
    if( worldModel_.board[x0][y].type != Type::EMPTY ){
      return false;
    }
  }
  return true;
}

bool GameLogic::validFrom(Command command, int32_t playerId){
  if( command.coordFrom.x < 0 || command.coordFrom.x > 7 )
    return false;
  if( command.coordFrom.y < 0 || command.coordFrom.y > 7 )
    return false;

  if( playerId == whitePlayerId_ ){
    return  worldModel_.board[command.coordFrom.x][command.coordFrom.y].type != Type::EMPTY
            && worldModel_.board[command.coordFrom.x][command.coordFrom.y].owner == PlayerColor::WHITE;
  } 
  else {
    return  worldModel_.board[command.coordFrom.x][command.coordFrom.y].type != Type::EMPTY
            && worldModel_.board[command.coordFrom.x][command.coordFrom.y].owner == PlayerColor::BLACK;
  }
}

bool GameLogic::validMove(Command command, int32_t playerId){
  std::vector<Command> validMoves = getValidMoves(command.coordFrom);
  for( unsigned int i = 0; i < validMoves.size(); i++ ){
    if( equalCommand(command, validMoves[i]) ){
      return true;
    }
  }
  return false;
}

bool GameLogic::equalCommand(Command c1, Command c2){
  return  c1.coordFrom.x == c2.coordFrom.x
          && c1.coordFrom.y == c2.coordFrom.y
          && c1.coordTo.x == c2.coordTo.x
          && c1.coordTo.y == c2.coordTo.y;
}

std::vector<Command> GameLogic::getAllValidMovesOfPlayer(PlayerColor color){
  std::vector<Command> v;
  Coordinate coord;
  for( size_t i = 0; i < boardSize_; i++ ){
    for( size_t j = 0; j < boardSize_; j++ ){
      if( worldModel_.board[i][j].type != Type::EMPTY && worldModel_.board[i][j].owner == color ){
        coord.x = i;
        coord.y = j;
        std::vector<Command> validMoves = getValidMoves(coord);
        for( unsigned int k = 0; k < validMoves.size(); k++ ){
          v.push_back(validMoves[k]);
        }
      }
    }
  }
  return v;
}

std::vector<Command> GameLogic::getValidMoves(Coordinate coord){
  std::vector<Command> v;
  Command command;
  command.coordFrom = coord;

  switch(worldModel_.board[coord.x][coord.y].type) {
    // ################################ PAWN ################################
    case Type::PAWN:
      if( worldModel_.board[coord.x][coord.y].owner == PlayerColor::WHITE ){
        // Go forward 1 square
        command.coordTo.x = command.coordFrom.x-1;
        command.coordTo.y = command.coordFrom.y;
        if( isInsideTheBoard(command.coordTo) && isEmpty(command.coordTo) )
          v.push_back(command);

        // Go forward 2 squares
        command.coordTo.x = command.coordFrom.x-2;
        command.coordTo.y = command.coordFrom.y;
        if( isInsideTheBoard(command.coordTo) && isEmpty(command.coordTo) && coord.x == 6)
          v.push_back(command);

        // Go to right-diagonal square removing enemy piece
        command.coordTo.x = command.coordFrom.x-1;
        command.coordTo.y = command.coordFrom.y+1;
        if( isInsideTheBoard(command.coordTo)
            && hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)))
          v.push_back(command);

        // Go to left-diagonal square removing enemy piece
        command.coordTo.x = command.coordFrom.x-1;
        command.coordTo.y = command.coordFrom.y-1;
        if( isInsideTheBoard(command.coordTo)
            && hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)))
          v.push_back(command);

        // TODO: En passant
      }
      else {
        // Go forward 1 square
        command.coordTo.x = command.coordFrom.x+1;
        command.coordTo.y = command.coordFrom.y;
        if( isInsideTheBoard(command.coordTo) && isEmpty(command.coordTo) )
          v.push_back(command);

        // Go forward 2 squares
        command.coordTo.x = command.coordFrom.x+2;
        command.coordTo.y = command.coordFrom.y;
        if( isInsideTheBoard(command.coordTo) && isEmpty(command.coordTo) && coord.x == 1)
          v.push_back(command);

        // Go to left-diagonal square removing enemy piece
        command.coordTo.x = command.coordFrom.x+1;
        command.coordTo.y = command.coordFrom.y+1;
        if( isInsideTheBoard(command.coordTo)
            && hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)))
          v.push_back(command);

        // Go to right-diagonal square removing enemy piece
        command.coordTo.x = command.coordFrom.x+1;
        command.coordTo.y = command.coordFrom.y-1;
        if( isInsideTheBoard(command.coordTo)
            && hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)))
          v.push_back(command);

        // TODO: En passant
      }
      break;
    // ################################ TOWER ################################
    case Type::TOWER:
      addTowerMoves(coord, command, v);
      break;
    // ################################ HORSE ################################
    case Type::HORSE:
      command.coordTo.x = coord.x+1;
      command.coordTo.y = coord.y+2;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);

      command.coordTo.y = coord.y-2;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);

      command.coordTo.x = coord.x+2;
      command.coordTo.y = coord.y+1;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);

      command.coordTo.y = coord.y-1;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);

      command.coordTo.x = coord.x-1;
      command.coordTo.y = coord.y+2;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);

      command.coordTo.y = coord.y-2;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);

      command.coordTo.x = coord.x-2;
      command.coordTo.y = coord.y+1;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);

      command.coordTo.y = coord.y-1;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);
      break;
    // ################################ BISHOP ################################
    case Type::BISHOP:
      addBishopMoves(coord, command, v);
      break;
    // ################################ QUEEN ################################
    case Type::QUEEN:
      addTowerMoves(coord, command, v);
      addBishopMoves(coord, command, v);
      break;
    // ################################ KING ################################
    case Type::KING:
      command.coordTo.x = coord.x;
      command.coordTo.y = coord.y+1;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);
      command.coordTo.y = coord.y-1;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);

      command.coordTo.x = coord.x+1;
      command.coordTo.y = coord.y+1;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);
      command.coordTo.y = coord.y-1;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);

      command.coordTo.x = coord.x-1;
      command.coordTo.y = coord.y+1;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);
      command.coordTo.y = coord.y-1;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);

      command.coordTo.x = coord.x+1;
      command.coordTo.y = coord.y;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);
      command.coordTo.x = coord.x-1;
      if( isInsideTheBoard(command.coordTo) 
          && (isEmpty(command.coordTo) 
          || hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) ))
        v.push_back(command);
      break;
    default:
      break;
  }

  return v;
}

void GameLogic::addTowerMoves(Coordinate& coord, Command& command, std::vector<Command>& v){
  command.coordTo.y = coord.y;
  // x++ direction
  for( command.coordTo.x = coord.x+1; isInsideTheBoard(command.coordTo) && isEmpty(command.coordTo); command.coordTo.x = command.coordTo.x+1 )
    v.push_back(command);
  if( isInsideTheBoard(command.coordTo)
      && hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) )
    v.push_back(command);

  // x-- direction
  for( command.coordTo.x = coord.x-1; isInsideTheBoard(command.coordTo) && isEmpty(command.coordTo); command.coordTo.x = command.coordTo.x-1 )
    v.push_back(command);
  if( isInsideTheBoard(command.coordTo)
      && hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) )
    v.push_back(command);

  command.coordTo.x = coord.x;

  // y++ direction
  for( command.coordTo.y = coord.y+1; isInsideTheBoard(command.coordTo) && isEmpty(command.coordTo); command.coordTo.y = command.coordTo.y+1 )
    v.push_back(command);
  if( isInsideTheBoard(command.coordTo)
      && hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) )
    v.push_back(command);

  // y-- direction
  for( command.coordTo.y = coord.y-1; isInsideTheBoard(command.coordTo) && isEmpty(command.coordTo); command.coordTo.y = command.coordTo.y-1 )
    v.push_back(command);
  if( isInsideTheBoard(command.coordTo)
      && hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) )
    v.push_back(command);
}

void GameLogic::addBishopMoves(Coordinate& coord, Command& command, std::vector<Command>& v){
  // x++ direction, y++ direction
  for(  command.coordTo.x = coord.x+1, command.coordTo.y = coord.y+1
        ; isInsideTheBoard(command.coordTo) && isEmpty(command.coordTo)
        ; command.coordTo.x = command.coordTo.x+1, command.coordTo.y = command.coordTo.y+1 )
    v.push_back(command);
  if( isInsideTheBoard(command.coordTo)
      && hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) )
    v.push_back(command);

  // x++ direction, y-- direction
  for(  command.coordTo.x = coord.x+1, command.coordTo.y = coord.y-1
        ; isInsideTheBoard(command.coordTo) && isEmpty(command.coordTo)
        ; command.coordTo.x = command.coordTo.x+1, command.coordTo.y = command.coordTo.y-1 )
    v.push_back(command);
  if( isInsideTheBoard(command.coordTo)
      && hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) )
    v.push_back(command);

  // x-- direction, y++ direction
  for(  command.coordTo.x = coord.x-1, command.coordTo.y = coord.y+1
        ; isInsideTheBoard(command.coordTo) && isEmpty(command.coordTo)
        ; command.coordTo.x = command.coordTo.x-1, command.coordTo.y = command.coordTo.y+1 )
    v.push_back(command);
  if( isInsideTheBoard(command.coordTo)
      && hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) )
    v.push_back(command);

  // x-- direction, y-- direction
  for(  command.coordTo.x = coord.x-1, command.coordTo.y = coord.y-1
        ; isInsideTheBoard(command.coordTo) && isEmpty(command.coordTo)
        ; command.coordTo.x = command.coordTo.x-1, command.coordTo.y = command.coordTo.y-1 )
    v.push_back(command);
  if( isInsideTheBoard(command.coordTo)
      && hasPieceOfColor(command.coordTo, getOpositeColor(worldModel_.board[coord.x][coord.y].owner)) )
    v.push_back(command);
}

bool GameLogic::hasPieceOfColor(Coordinate coord, PlayerColor color){
  return  worldModel_.board[coord.x][coord.y].type != Type::EMPTY
          && worldModel_.board[coord.x][coord.y].owner == color;
}

PlayerColor GameLogic::getOpositeColor(PlayerColor color){
  if( color == PlayerColor::WHITE ){
    return PlayerColor::BLACK;
  }
  else{
    return PlayerColor::WHITE;
  }
}

bool GameLogic::isEmpty(Coordinate coord) {
  return worldModel_.board[coord.x][coord.y].type == Type::EMPTY;
}

bool GameLogic::isInsideTheBoard(Coordinate coord) {
  return coord.x >= 0 && coord.x < (int) boardSize_ && coord.y >= 0 && coord.y < (int) boardSize_;
}

bool GameLogic::playerInCheck(int32_t playerId){
  PlayerColor playerColor = (playerId == whitePlayerId_? PlayerColor::WHITE : PlayerColor::BLACK );
  PlayerColor enemyColor = (playerId == whitePlayerId_? PlayerColor::BLACK : PlayerColor::WHITE );

  std::vector<Command> allValidMovesOfEnemy = getAllValidMovesOfPlayer(enemyColor);

  Coordinate kingCoord = getKingCoordinateOfPlayer(playerColor);

  for( unsigned int i = 0; i < allValidMovesOfEnemy.size(); i++ ){
    if( equalCoordinate(kingCoord, allValidMovesOfEnemy[i].coordTo) ){
      return true;
    }
  }
  return false;
}

// It's supossed that the move can be doed
bool GameLogic::playerInCheckAfterCommand(Command command, int32_t playerId){
  makeWorldModelBackup();
  movePiece(command, playerId);
  bool playerInCheckAfterCommand = playerInCheck(playerId);
  restoreWorldModelFromBackup();
  return playerInCheckAfterCommand;
}

void GameLogic::makeWorldModelBackup(){
  worldModelBackup_ = worldModel_;
}

void GameLogic::restoreWorldModelFromBackup(){
  worldModel_ = worldModelBackup_;
}

bool GameLogic::equalCoordinate(Coordinate c1, Coordinate c2){
  return c1.x == c2.x && c1.y == c2.y;
}

Coordinate GameLogic::getKingCoordinateOfPlayer(PlayerColor color){
  Coordinate coord;
  for( unsigned int i = 0; i < boardSize_; i++ ){
    for(unsigned int j = 0; j < boardSize_; j++ ){
      if( worldModel_.board[i][j].type == Type::KING && worldModel_.board[i][j].owner == color ){
        coord.x = i;
        coord.y = j;
        return coord;
      }
    }
  }
  return coord;
}

GameDescription GameLogic::getGameDescription(int32_t playerId) const {
  GameDescription gameDescription;
  gameDescription.myColor = (playerId == whitePlayerId_) ?
    PlayerColor::WHITE : PlayerColor::BLACK;
  return gameDescription;
}

// It's supossed that the move can be doed
void GameLogic::movePiece(Command command, int32_t playerId) {
  // Rock move
  if( playerId == whitePlayerId_ ){
    if( command.smallRock ){
      Command kingCommand;
      kingCommand.coordFrom.x = 7;
      kingCommand.coordFrom.y = 4;
      kingCommand.coordTo.x = 7;
      kingCommand.coordTo.y = 6;
      movePiece(kingCommand, playerId);
      Command towerCommand;
      towerCommand.coordFrom.x = 7;
      towerCommand.coordFrom.y = 7;
      towerCommand.coordTo.x = 7;
      towerCommand.coordTo.y = 5;
      movePiece(towerCommand, playerId);
      return;
    }
    else if( command.bigRock ){
      Command kingCommand;
      kingCommand.coordFrom.x = 7;
      kingCommand.coordFrom.y = 4;
      kingCommand.coordTo.x = 7;
      kingCommand.coordTo.y = 2;
      movePiece(kingCommand, playerId);
      Command towerCommand;
      towerCommand.coordFrom.x = 7;
      towerCommand.coordFrom.y = 0;
      towerCommand.coordTo.x = 7;
      towerCommand.coordTo.y = 3;
      movePiece(towerCommand, playerId);
      return;
    }
    else if( validEnPassant_ ){
      Piece emptyPiece;
      emptyPiece.type = Type::EMPTY;
      worldModel_.board[enPassant_.pawnX][enPassant_.pawnY] = emptyPiece;
    }
  }
  else { // black player move
    if( command.smallRock ){
      Command kingCommand;
      kingCommand.coordFrom.x = 0;
      kingCommand.coordFrom.y = 4;
      kingCommand.coordTo.x = 0;
      kingCommand.coordTo.y = 6;
      movePiece(kingCommand, playerId);
      Command towerCommand;
      towerCommand.coordFrom.x = 0;
      towerCommand.coordFrom.y = 7;
      towerCommand.coordTo.x = 0;
      towerCommand.coordTo.y = 5;
      movePiece(towerCommand, playerId);
      return;
    }
    else if( command.bigRock ){
      Command kingCommand;
      kingCommand.coordFrom.x = 0;
      kingCommand.coordFrom.y = 4;
      kingCommand.coordTo.x = 0;
      kingCommand.coordTo.y = 2;
      movePiece(kingCommand, playerId);
      Command towerCommand;
      towerCommand.coordFrom.x = 0;
      towerCommand.coordFrom.y = 0;
      towerCommand.coordTo.x = 0;
      towerCommand.coordTo.y = 3;
      movePiece(towerCommand, playerId);
      return;
    }
    else if( validEnPassant_ ){
      Piece emptyPiece;
      emptyPiece.type = Type::EMPTY;
      worldModel_.board[enPassant_.pawnX][enPassant_.pawnY] = emptyPiece;
    }
  }

  // Normal move
  Piece emptyPiece;
  emptyPiece.type = Type::EMPTY;
  worldModel_.board[command.coordTo.x][command.coordTo.y] =
    worldModel_.board[command.coordFrom.x][command.coordFrom.y];
  worldModel_.board[command.coordFrom.x][command.coordFrom.y] =
    emptyPiece;

  // Pawn promotion
  if( playerId == whitePlayerId_ ){
    if( worldModel_.board[command.coordTo.x][command.coordTo.y].type == Type::PAWN
        && command.coordTo.x == 0 ) {
      worldModel_.board[command.coordTo.x][command.coordTo.y].type = promoTypeToType(command.promoteTo);
    }
  }
  else { // black player move
    if( worldModel_.board[command.coordTo.x][command.coordTo.y].type == Type::PAWN
        && command.coordTo.x == 7 ) {
      worldModel_.board[command.coordTo.x][command.coordTo.y].type = promoTypeToType(command.promoteTo);
    }
  }
}

Type GameLogic::promoTypeToType(PromoType promoType){
  switch(promoType){
    case PromoType::PROMOTE_TO_TOWER:
      return Type::TOWER;
    case PromoType::PROMOTE_TO_HORSE:
      return Type::HORSE;
    case PromoType::PROMOTE_TO_BISHOP:
      return Type::BISHOP;
    default:
      return Type::QUEEN;
  }
}

void GameLogic::setRockVars(Command command, int32_t playerId){
  if( playerId == whitePlayerId_ ){
    if( command.smallRock ){
      rock_.whiteKingMoved = true;
      rock_.whiteRightTowerMoved = true;
      return;
    } 
    else if( command.bigRock ){
      rock_.whiteKingMoved = true;
      rock_.whiteLeftTowerMoved = true;
      return;
    } 
  } 
  else { // black player command
    if( command.smallRock ){
      rock_.blackKingMoved = true;
      rock_.blackRightTowerMoved = true;
      return;
    } 
    else if( command.bigRock ){
      rock_.blackKingMoved = true;
      rock_.blackLeftTowerMoved = true;
      return;
    } 
  }

  if( worldModel_.board[command.coordFrom.x][command.coordFrom.y].type == Type::KING ){
    if( worldModel_.board[command.coordFrom.x][command.coordFrom.y].owner == PlayerColor::BLACK ){
      rock_.blackKingMoved = true;
    }
    else {
      rock_.whiteKingMoved = true;
    }
  }
  else if( worldModel_.board[command.coordFrom.x][command.coordFrom.y].type == Type::TOWER ){
    if( worldModel_.board[command.coordFrom.x][command.coordFrom.y].owner == PlayerColor::BLACK ){
      if( !rock_.blackLeftTowerMoved && command.coordFrom.x == 0 && command.coordFrom.y == 0 ){
        rock_.blackLeftTowerMoved = true;
      }
      else if( !rock_.blackRightTowerMoved && command.coordFrom.x == 0 && command.coordFrom.y == 8-1 ){
        rock_.blackRightTowerMoved = true;
      }
    }
    else {
      if( !rock_.whiteLeftTowerMoved && command.coordFrom.x == 8-1 && command.coordFrom.y == 0 ){
        rock_.whiteLeftTowerMoved = true;
      }
      else if( !rock_.whiteRightTowerMoved && command.coordFrom.x == 8-1 && command.coordFrom.y == 8-1 ){
        rock_.whiteRightTowerMoved = true;
      }
    }
  }
}

WorldModel GameLogic::getWorldModel() const {
  return worldModel_;
}

std::vector<Command>& GameLogic::getMoveList(int32_t playerId) {
  return moveList_;
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
