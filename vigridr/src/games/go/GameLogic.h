#ifndef VIGRIDR_SERVER_GAME_LOGIC_H
#define VIGRIDR_SERVER_GAME_LOGIC_H

#include "../thrifts/gen-cpp/Command_types.h"
#include "../thrifts/gen-cpp/WorldModel_types.h"
#include "../thrifts/gen-cpp/GameDescription_types.h"
#include "../thrifts/gen-cpp/GameResult_types.h"
#include <string>
#include <vector>

namespace mjollnir { namespace vigridr {

typedef enum Player_t {
  BLACK = 'B',
  WHITE = 'W',
  EMPTY = '.',
  DOMI = 'D'
} Player;

struct TotalWorldModel {};

typedef struct Group_t{
  int size;
  int liberties;
  Player player;
  Player owner;
} Group;

/************************/
/************************/
/************************/
/************************/
/************************/
/************************/
/****     STONE      ****/
/************************/
/************************/
/************************/
/************************/
/************************/
/************************/

class Stone {

 private:
  int x;
  int y;
  Player player;
  bool Counted; 

 public:
  Stone() {Counted = 0;};
  ~Stone() {};
  
  Stone(Player player, int x, int y);

  //control for the counted flag
  void clearCounted();
  void setCounted();
  bool isCounted();
  
  //get/set and boolean test for the Player
  void setPlayer(Player player);
  char getPlayer();
  bool isPlayer(Player target);
  
  //Getter/setters for coordinates
  void setY(int y);
  void setX(int x);
  int getX();
  int getY();
};

/************************/
/************************/
/************************/
/************************/
/************************/
/************************/
/****   GAME_LOGIC   ****/
/************************/
/************************/
/************************/
/************************/
/************************/
/************************/

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
  void setTableCoordinate(const Coordinate& coordinate, Marker marker);
  /**
   * Returns the game result for player with id 'id'
   */
  GameResult createGameResult(std::string result, int32_t id);

 private:
  void setTableCoordinate_(const Coordinate& coordinate, Marker marker);
  bool checkTableCoordinate_(const Coordinate& coordinate, Marker marker);
  bool isPassMove_(int32_t x, int32_t y);
  bool isValidMove_(int32_t x, int32_t y);
  bool isPlayer_(int32_t x, int32_t y, Marker marker);
  bool isKo_(int32_t x, int32_t y);
  void updateWorldModel_();
  bool isOnBoard_(int32_t x, int32_t y);
  bool randomPlay_(int32_t playerId);
  bool matchFinished_();
  void determineWinner_();
  int step_( Player player, const Coordinate& coordinate );
  bool effectEnemies_( Player enemy, int x, int y );
  int countNeighbors_( Player target, int x, int y );
  void clearMarks_();
  void clearGroup_( Player player, int x, int y );
  Group getGroup_( int x, int y, Group currGroup );
  Player scoreArea_();
  WorldModel worldModel_;
  TotalWorldModel twm_;
  int32_t player1_, player2_;
  int32_t koPosX_, koPosY_;
  int32_t bMove_, wMove_;
  std::string winner_;
  bool hasFinished_;
  Stone *** grid_; //2d array of stone pointers
  int32_t scoreBlack_;
  int32_t scoreWhite_;
  int32_t blackPrisoners_;
  int32_t whitePrisoners_;
  int32_t blackStonesNumber_;
  int32_t whiteStonesNumber_;
  const int32_t boardSize_ = 9;
  const int32_t initialBlackStonesNumber_ = 41;
  const int32_t initialWhiteStonesNumber_ = 40;
  const size_t numberOfPlayers_ = 2;
};

}}  // namespaces

#endif  // VIGRIDR_SERVER_GAME_LOGIC_H
