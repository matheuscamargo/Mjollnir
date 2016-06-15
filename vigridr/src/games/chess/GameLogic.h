#ifndef VIGRIDR_SERVER_GAME_LOGIC_H
#define VIGRIDR_SERVER_GAME_LOGIC_H

#include "../thrifts/gen-cpp/Command_types.h"
#include "../thrifts/gen-cpp/WorldModel_types.h"
#include "../thrifts/gen-cpp/GameDescription_types.h"
#include "../thrifts/gen-cpp/GameResult_types.h"
#include <string>

namespace mjollnir { namespace vigridr {

struct TotalWorldModel {};

class GameLogic {
 public:
  bool shouldPrintWorldModel(int32_t playerId);
  bool shouldIncrementCycle(int32_t playerId);
  GameLogic(const std::vector<int32_t> &playerIds);
  bool update(Command command, int32_t playerId);
  WorldModel getWorldModel() const;
  bool isFinished() const;
  std::string getWinner() const;
  GameDescription getGameDescription(int32_t playerId) const;
  TotalWorldModel getTotalWorldModel() const;
  size_t getNumberOfPlayers() const;
  void setFirstPlayer(int32_t playerId){};
  /**
   *  Specific function to use at GameLogicTest test suite
   */
  void setHasFinished(bool value);
  /**
   *  Specific function to use at GameLogicTest test suite
   */
  void setWinner(std::string value);
  /**
   *  Specific function to use at GameLogicTest test suite.
   *  Internally should use setTableCoordinate()
   */
  void setTableCoordinate(const Coordinate& coordinate, Piece piece);
  /**
   * Returns the game result for player with id 'id'
   */
  GameResult createGameResult(std::string result, int32_t id);

 private:
  void movePiece(Command command);
  void setTableCoordinate_(const Coordinate& coordinate, Piece piece);
  Piece createPiece(Type type, PlayerColor owner);
  WorldModel worldModel_;
  TotalWorldModel twm_;
  int32_t player1_, player2_;
  std::string winner_;
  bool hasFinished_;
  const size_t boardSize_ = 8;
  const size_t numberOfPlayers_ = 2;
  int tempGameCounter = 27;
};

}}  // namespaces

#endif  // VIGRIDR_SERVER_GAME_LOGIC_H
