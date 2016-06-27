#include "GameManager.h"

#include <stdexcept>
#include <cstdlib>
#include <ctime>

#include <gflags/gflags.h>

#include "../utils/Log.h"
#include "../utils/MapUtils.h"
#include "GameType.h"
#include "GameConfig.h"
#include "GameLogger.h"

DEFINE_string(player1, "p1", "First player's id (to be logged).");
DEFINE_string(player2, "p2", "Second player's id (to be logged).");

namespace mjollnir { namespace vigridr {

void PlayerTurnData::init(int32_t id) {
  setId(id);
  clearCommand();
}

void PlayerTurnData::setCommand(Command command, time_point time) {
  isCommandSet_ = true;
  lastUpdateTime_ = time;
  command_ = std::move(command);
}

void PlayerTurnData::clearCommand() {
  isCommandSet_ = false;
}

void PlayerTurnData::setIsTurn(bool isTurn) {
  isTurn_ = isTurn;
}

void PlayerTurnData::setGameResult(GameResult result) {
  result_ = result;
}

void PlayerTurnData::setMoveList(std::vector<Command>& moveList) {
  moveList_ = moveList;
}

GameManager::GameManager(const std::vector<int32_t> &playerIds)
  : gameLogic_(playerIds) {

  for (size_t i = 0; i < playerIds.size(); i++) {
    idToIdx_[playerIds[i]] = i;
    idxToId_[i] = playerIds[i];
  }

  turn_ = 0;
  srand(time(NULL));
  initializeGame(playerIds);
  updaterThread_ = std::thread([this]() {
    updaterTask();
    // calling game end handlers callback
    for (auto& handler : gameEndHandlers_) {
      handler();
    }
  });
}

GameManager::~GameManager() {
  updaterThread_.join();
}

void GameManager::finalizeGame() {
  gameInfo_.gameStatus = GameStatus::FINISHED;
  gameInfo_.worldModel = gameLogic_.getWorldModel();
  {
    // Unlocks all client threads that were waiting for some condition
    {
      std::unique_lock<std::mutex> lockUpdateFlag(updateFlagMutex_);
      std::unique_lock<std::mutex> lock0(playerMutex_[0]);
      std::unique_lock<std::mutex> lock1(playerMutex_[1]);
    
      playerTurnData_[0].setIsTurn(true);
      playerTurnData_[1].setIsTurn(true);
      updateFlag_ = false;
    }
    condUpdate_.notify_all();
    condTurn_.notify_all();
  }
  
  std::chrono::seconds sleepTime(2);
  std::this_thread::sleep_for(sleepTime);
  GameLogger::printWorldModel(gameInfo_.worldModel, gameLogic_.getTotalWorldModel());
  GameLogger::logWorldModel(gameInfo_.worldModel, gameLogic_.getTotalWorldModel());
  GameLogger::flushLog();
}

void GameManager::initializeGame(const std::vector<int32_t> &playerIds) {
  std::unique_lock<std::mutex> lock0(playerMutex_[0], std::defer_lock);
  std::unique_lock<std::mutex> lock1(playerMutex_[1], std::defer_lock);
  std::unique_lock<std::mutex> lock2(gameInfoMutex_, std::defer_lock);
  std::unique_lock<std::mutex> lockUpdateFlag(updateFlagMutex_, std::defer_lock);
  std::lock(lock0, lock1, lock2, lockUpdateFlag);

  gameInfo_.worldModel = gameLogic_.getWorldModel();
  for (size_t i = 0; i < kMaxPlayers; ++i) {
      playerTurnData_[i].setMoveList(gameLogic_.getMoveList(playerTurnData_[i].getId()));
  }
  GameLogger::logGameDescription(gameLogic_.getGameDescription(playerIds[0]),
                                 FLAGS_player1,
                                 gameLogic_.getGameDescription(playerIds[1]),
                                 FLAGS_player2);
  playerTurnData_[0].init(playerIds[0]);
  playerTurnData_[1].init(playerIds[1]);
  playerTurnData_[0].setIsTurn(true);
  playerTurnData_[1].setIsTurn(true);

  // This flag is set true, so every client requisition will be locked until game starts
  updateFlag_ = true;
  if (config::gameType == GameType::TURN) {
    int32_t firstPlayer = rand() % 2;
    playerTurnData_[firstPlayer].setIsTurn(false);
    // If firstPlayer == 0, then playerTurnData_[0] = false and playerTurnData_[1] = true
    // Since nextTurn() invert the player turn and nextTurn() is called on the begining 
    // of updateTask(), in the case that firstPlayer == 0, the first move goes to playerId0
    gameLogic_.setFirstPlayer(firstPlayer == 0 ? playerIds[0] : playerIds[1] );
  }
  gameInfo_.cycle = 0;
  gameInfo_.gameStatus = GameStatus::WAITING;
  timer_.startInitializationCycle();
}

void GameManager::nextTurn() {
  {
    std::unique_lock<std::mutex> lock0(playerMutex_[0], std::defer_lock);
    std::unique_lock<std::mutex> lock1(playerMutex_[1], std::defer_lock);
    std::unique_lock<std::mutex> lock2(gameInfoMutex_, std::defer_lock);
    std::lock(lock0, lock1, lock2);
    if (config::gameType == GameType::TURN) {
      for (size_t i = 0; i < kMaxPlayers; ++i) {
          playerTurnData_[i].setIsTurn(!playerTurnData_[i].isTurn());
      }
    }
    gameInfo_.gameStatus = GameStatus::RUNNING;
    gameInfo_.worldModel = gameLogic_.getWorldModel();
    for (size_t i = 0; i < kMaxPlayers; ++i) {
        playerTurnData_[i].setMoveList(gameLogic_.getMoveList(playerTurnData_[i].getId()));
    }
    int32_t playerOnTurn = playerTurnData_[0].isTurn() ? playerTurnData_[0].getId() : playerTurnData_[1].getId();
    if (gameLogic_.shouldIncrementCycle(playerOnTurn)) {
      gameInfo_.cycle++;
    }
    if (gameLogic_.shouldPrintWorldModel(playerOnTurn)){
      GameLogger::printWorldModel(gameInfo_.worldModel, gameLogic_.getTotalWorldModel());
    }
    GameLogger::logWorldModel(gameInfo_.worldModel, gameLogic_.getTotalWorldModel());
  }
  // Notify client threads that were waiting for their turns
  condTurn_.notify_all();
  timer_.startCycle();
}

bool
checkCommands(const std::array<PlayerTurnData, kMaxPlayers>& playerTurnData,
              int32_t& correctPlayer) {
  int32_t wrongPlayerCount = 0;
  for (size_t i = 0; i < kMaxPlayers; ++i) {
    if (playerTurnData[i].isTurn() && !playerTurnData[i].isCommandSet()) {
      LOG("[ERROR]: No command set for player %d", i);
      wrongPlayerCount++;
    } else {
      correctPlayer = playerTurnData[i].getId();
    }
  }
  if(wrongPlayerCount > 1) correctPlayer = -1;
  return wrongPlayerCount == 0;
}

void
clearCommands(std::array<PlayerTurnData, kMaxPlayers>& playerTurnData) {
  if(playerTurnData[0].isTurn()) { playerTurnData[0].clearCommand(); }
  if(playerTurnData[1].isTurn()) { playerTurnData[1].clearCommand(); }
}


// GameManager::updaterTask is a task that is executed to update the game every cycle
void GameManager::updaterTask() {
  timer_.sleepUntilInitializationTime();

  while (true) {
    nextTurn();  // initialize next turn
    turn_++;

    {
      std::unique_lock<std::mutex> lockUpdateFlag(updateFlagMutex_, std::defer_lock);
      {
        lockUpdateFlag.lock();
        if(updateFlag_) {
          updateFlag_ = false;
          lockUpdateFlag.unlock();
          condUpdate_.notify_all();
        }
      }
    }
    {
      std::unique_lock<std::mutex> lockUpdateFlag(updateFlagMutex_, std::defer_lock);
      lockUpdateFlag.lock();
      if(updateFlag_ == false) {
        // Blocks this updaterTask until one or more clients send commands (depends if turn based or not)
        // If no one sends command, wake up after update time limit is reached
        condUpdate_.wait_for(lockUpdateFlag, std::chrono::milliseconds(timer_.getPlayerUpdateTimeLimit()), [this](){return updateFlag_;});
      }
    }
    LOG("Updating...");
    std::array<PlayerTurnData, kMaxPlayers> movements;
    {
      std::unique_lock<std::mutex> lock0(playerMutex_[0], std::defer_lock);
      std::unique_lock<std::mutex> lock1(playerMutex_[1], std::defer_lock);
      // Locks player 1 and 2 simultaneously so that there is no dead lock
      std::lock(lock0, lock1);
      // Check wheather the players sent the command if just one sent gets him
      int32_t correctPlayer;
      bool errorHappened = !checkCommands(playerTurnData_, correctPlayer);
      if (errorHappened) {
        if(gameLogic_.getNumberOfPlayers() == 1) {
          std::cout << gameLogic_.getWinner();
          playerTurnData_[0].setGameResult(gameLogic_.createGameResult(gameLogic_.getWinner(), idxToId_[0]));
        } else {
          std::cout << correctPlayer;
          for (size_t idx = 0; idx < playerTurnData_.size(); ++idx) {
            playerTurnData_[idx].setGameResult(gameLogic_.createGameResult(std::to_string(correctPlayer), idxToId_[idx]));
          }
        }
        break;
      }
      movements = playerTurnData_;  // copy assignment operation
      // now we can release the locks so that other threads can change
      // playerTurnData without compromising the game update task
      // btw playerTurnData is the data sent from a player in a given turn
      clearCommands(playerTurnData_);
    }
    // sorting the player movements by update time (who sent first moves first)
    std::sort(movements.begin(), movements.end(),
      [](const PlayerTurnData& a, const PlayerTurnData& b) {
        return a.getLastUpdatedTime() < b.getLastUpdatedTime();
    });

    // only execute movements if it is the player's turn
    int32_t countWrongPlayers = 0;  // how many players sent invalid command
    int32_t correctPlayer = 0;  // if one is wrong this variable
    for (auto& playerMove : movements) {
      if (!playerMove.isTurn()) {
        correctPlayer = playerMove.getId();  // if it is not my turn i'm valid
        continue;
      }
      bool validCommand =
        gameLogic_.update(playerMove.getCommand(), playerMove.getId());
      if (!validCommand) {
        countWrongPlayers++;
      } else {
        correctPlayer = playerMove.getId();  // valid command
      }
    }

    bool finished = gameLogic_.isFinished();
    std::string winner = gameLogic_.getWinner();

    if(gameLogic_.getNumberOfPlayers() == 1) {
      if(countWrongPlayers > 0) {
        winner = gameLogic_.getWinner();
        finished = true;
      }
    } else {
      // if one player sent an invalid command the other one wins
      if(countWrongPlayers == 1) {
        winner = std::to_string(correctPlayer);
        finished = true;

      } else if (countWrongPlayers == 2) {
        winner = "-1";
        finished = true;
      }
    }
    

    if(turn_ >= kMaxTurns){
      finished = true;
      // This only works for wumpus
      // TODO: implement for 2 players game
      // TODO: get kMaxTurns from game logic so it can be different for each game
      winner = gameLogic_.getWinner();
    }

    if (finished) {
      LOG("Winner is %s", winner.c_str());

      // print winner so that the caller knows how game ended
      std::cout << winner;
      for (size_t idx = 0; idx < playerTurnData_.size(); ++idx) {
        playerTurnData_[idx].setGameResult(gameLogic_.createGameResult(winner, idxToId_[idx]));
      }
      break;
    }
  }
  
  // game ends
  finalizeGame();
}

bool
checkStepFinished(const std::array<PlayerTurnData, kMaxPlayers>& playerTurnData) {
  int32_t howManySentCommand = 0;
  for (size_t i = 0; i < kMaxPlayers; ++i) {
    if (playerTurnData[i].isCommandSet()) howManySentCommand++;
  }
  if (config::gameType == GameType::TURN) {
    return (howManySentCommand >= 1);
  }
  else if (config::gameType == GameType::CONTINUOUS) {
    return (howManySentCommand == kMaxPlayers);
  }
  return false;
}

CommandStatus GameManager::update(const Command& command, int32_t playerId) {
  int32_t idx = utils::getDefault(idToIdx_, playerId, -1);
  CHECK((idx == 0 || idx == 1), "Unknown player with id %d", idx);
  {
    std::unique_lock<std::mutex> lockPlayerTurnData0(playerMutex_[0]);
    std::unique_lock<std::mutex> lockPlayerTurnData1(playerMutex_[1]);
    playerTurnData_[idx].setCommand(command, clock::now());
    // Check if all players needed to run next update already sent a command
    bool stepFinished = checkStepFinished(playerTurnData_);
    // If yes, set updateFlag to true and notify all threads waiting this condition
    // Notified thread will be updaterTask thread
    if (stepFinished) {
      std::unique_lock<std::mutex> lockUpdateFlag(updateFlagMutex_, std::defer_lock);
      lockUpdateFlag.lock();
      updateFlag_ = true;
      lockUpdateFlag.unlock();
      condUpdate_.notify_all();
    }
  }
  return CommandStatus::SUCCESS;
}

void GameManager::getGameInfo(GameInfo& gameInfo, int32_t playerId) {
  const size_t kInvalid = 10000;
  size_t idx = utils::getDefault(idToIdx_, playerId, kInvalid);
  CHECK(idx != kInvalid, "Unknown player with id %d.", idx);
  {
    // Check if: 1 updateFlag is false
    //           2 it's player turn and he hasn't sent a command
    // If one of the conditions is false, then lock player thread
    bool canPlay = false;
    while(!canPlay) {
      canPlay = true;
      {
        std::unique_lock<std::mutex> lockUpdateFlag(updateFlagMutex_, std::defer_lock);
        lockUpdateFlag.lock();
        if(updateFlag_) {
          canPlay = false;
          condUpdate_.wait(lockUpdateFlag, [this](){
            return updateFlag_ == false;});
        }
      }
      {
        std::unique_lock<std::mutex> lockPlayerTurnData(playerMutex_[idx], std::defer_lock);
        lockPlayerTurnData.lock();
        if(playerTurnData_[idx].isTurn() == false || playerTurnData_[idx].isCommandSet() == true) {
          canPlay = false;
          condTurn_.wait(lockPlayerTurnData, [this, idx](){
            return (playerTurnData_[idx].isTurn() && !playerTurnData_[idx].isCommandSet());});
        }
      }

    }
  }
  {
    std::unique_lock<std::mutex> lock(gameInfoMutex_);
    gameInfo = gameInfo_;
  }
  {
    std::unique_lock<std::mutex> lock(playerMutex_[idx]);
    gameInfo.isMyTurn = playerTurnData_[idx].isTurn();
    gameInfo.gameResult = playerTurnData_[idx].getGameResult();
    gameInfo.moveList = playerTurnData_[idx].getMoveList();
  }
}

void GameManager::getGameInit(GameInit& gameInit, int32_t playerId) {
  gameInit.gameDescription = gameLogic_.getGameDescription(playerId);
}

void GameManager::onGameEnd(std::function<void ()> handler) {
  std::unique_lock<std::mutex> lock(gameEndHandlersMutex_);
  gameEndHandlers_.push_back(handler);
}

}}

