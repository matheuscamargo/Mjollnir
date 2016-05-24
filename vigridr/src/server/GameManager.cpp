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

GameManager::GameManager(int32_t playerId0, int32_t playerId1)
  : idToIdx_({{playerId0, 0}, {playerId1, 1}}),
    idxToId_({{playerId0, playerId1}}),
    gameLogic_(playerId0, playerId1) {
  turn_ = 0;
  srand(time(NULL));
  initializeGame(playerId0, playerId1);
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
    LOG("Fim de jogo, desbloqueia clientes");
    std::unique_lock<std::mutex> lockUpdateFlag(updateFlagMutex_, std::defer_lock);
    std::unique_lock<std::mutex> lock0(playerMutex_[0], std::defer_lock);
    std::unique_lock<std::mutex> lock1(playerMutex_[1], std::defer_lock);
    {
      lockUpdateFlag.lock();
      lock0.lock();
      lock1.lock();
      playerTurnData_[0].setIsTurn(true);
      playerTurnData_[1].setIsTurn(true);
      updateFlag_ = false;
      lockUpdateFlag.unlock();
      lock0.unlock();
      lock1.unlock();
      condUpdate_.notify_all();
      condTurn_.notify_all();
      
    }
  }
  //Workaround solution so that clients have time to finish by themselves
  //Before the server is killed
  std::chrono::seconds sleepTime(1);
  std::this_thread::sleep_for(sleepTime);
  GameLogger::printWorldModel(gameInfo_.worldModel, gameLogic_.getTotalWorldModel());
  GameLogger::logWorldModel(gameInfo_.worldModel, gameLogic_.getTotalWorldModel());
  GameLogger::flushLog();
}

void GameManager::initializeGame(int32_t playerId0, int32_t playerId1) {
  std::unique_lock<std::mutex> lock0(playerMutex_[0], std::defer_lock);
  std::unique_lock<std::mutex> lock1(playerMutex_[1], std::defer_lock);
  std::unique_lock<std::mutex> lock2(gameInfoMutex_, std::defer_lock);
  std::unique_lock<std::mutex> lockUpdateFlag(updateFlagMutex_, std::defer_lock);
  std::lock(lock0, lock1, lock2, lockUpdateFlag);

  gameInfo_.worldModel = gameLogic_.getWorldModel();
  GameLogger::logGameDescription(gameLogic_.getGameDescription(playerId0),
                                 FLAGS_player1,
                                 gameLogic_.getGameDescription(playerId1),
                                 FLAGS_player2);
  playerTurnData_[0].init(playerId0);
  playerTurnData_[1].init(playerId1);
  playerTurnData_[0].setIsTurn(true);
  playerTurnData_[1].setIsTurn(true);

  updateFlag_ = true;
  if (config::gameType == GameType::TURN) {
    playerTurnData_[rand() % 2].setIsTurn(false);
  }
  gameInfo_.cycle = 0;
  gameInfo_.gameStatus = GameStatus::WAITING;
  timer_.startInitializationCycle();
}

// GetGameInfo
// updateFlag_ = true, entao ta no updaterTask e tem que ficar bloqueado
// updateFlag_ = false, entao ta na hora de receber os comandos mesmo

//
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
    int32_t playerOnTurn = playerTurnData_[0].isTurn() ? playerTurnData_[0].getId() : playerTurnData_[1].getId();
    if (gameLogic_.shouldIncrementCycle(playerOnTurn)) {
      gameInfo_.cycle++;
    }
    if (gameLogic_.shouldPrintWorldModel(playerOnTurn)){
      GameLogger::printWorldModel(gameInfo_.worldModel, gameLogic_.getTotalWorldModel());
    }
    GameLogger::logWorldModel(gameInfo_.worldModel, gameLogic_.getTotalWorldModel());
  }
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


// updater task is a task that is executed to update the game every cycle
void GameManager::updaterTask() {

  timer_.sleepUntilInitializationTime();
  while (true) {
    nextTurn();  // initialize next turn
    turn_++;

    {
      LOG("updateFlag_ = false");
      std::unique_lock<std::mutex> lockUpdateFlag(updateFlagMutex_, std::defer_lock);
      {
        lockUpdateFlag.lock();
        if(updateFlag_) {
          updateFlag_ = false;
          lockUpdateFlag.unlock();
          condUpdate_.notify_all();
          LOG("Notifying threads");
        }
      }
    }
    {
      std::unique_lock<std::mutex> lockUpdateFlag(updateFlagMutex_, std::defer_lock);
      lockUpdateFlag.lock();
      if(updateFlag_ == false) {
        LOG("Blocking updaterTask");
        //blocks this updaterTask until one or more clients send commands (depends if turn based or not)
        LOG("Will sleep for %d", timer_.getPlayerUpdateTimeLimit());
        condUpdate_.wait_for(lockUpdateFlag, std::chrono::milliseconds(timer_.getPlayerUpdateTimeLimit()), [this](){return updateFlag_;});
        LOG("Remaining sleep time %d", timer_.getPlayerUpdateTimeLimit());
        LOG("Unblocking updaterTask");
      }
    }
    LOG("Updating...");
    std::array<PlayerTurnData, kMaxPlayers> movements;
    {
      std::unique_lock<std::mutex> lock0(playerMutex_[0], std::defer_lock);
      std::unique_lock<std::mutex> lock1(playerMutex_[1], std::defer_lock);
      // locks player 1 and 2 simultaneously so that there is no dead lock
      std::lock(lock0, lock1);
      // check wheather the players sent the command if just one sent gets him
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
    LOG("Player %d sent a command", (int)idx);
    playerTurnData_[idx].setCommand(command, clock::now());
    bool stepFinished = checkStepFinished(playerTurnData_);
    if (stepFinished) {
      std::unique_lock<std::mutex> lockUpdateFlag(updateFlagMutex_, std::defer_lock);
      lockUpdateFlag.lock();
      updateFlag_ = true;
      LOG("updateFlag_ = True");
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
    LOG("Client getGameInfo %d", (int)idx);

    bool canPlay = false;
    while(!canPlay) {
      LOG("Player %d is starting loop", idx);
      canPlay = true;
      {
        std::unique_lock<std::mutex> lockUpdateFlag(updateFlagMutex_, std::defer_lock);
        lockUpdateFlag.lock();
        if(updateFlag_) {
          canPlay = false;
          LOG("Blocking client %d: updateFlag_ is true", (int)idx);
          condUpdate_.wait(lockUpdateFlag, [this](){
            return updateFlag_ == false;});
          LOG("Unblocking client %d: updateFlag is now false", (int)idx);
        }
      }
      {
        std::unique_lock<std::mutex> lockPlayerTurnData(playerMutex_[idx], std::defer_lock);
        lockPlayerTurnData.lock();
        if(playerTurnData_[idx].isTurn() == false || playerTurnData_[idx].isCommandSet() == true) {
          canPlay = false;
          LOG("Blocking client %d: It's not player turn", (int)idx);
          condTurn_.wait(lockPlayerTurnData, [this, idx](){
            return (playerTurnData_[idx].isTurn() && !playerTurnData_[idx].isCommandSet());});
          LOG("Unblocking client %d: It's now player turn", (int)idx);
        }
      }

    }
    LOG("Player %d is out of while loop", idx);
  }
  {
    std::unique_lock<std::mutex> lock(gameInfoMutex_);
    gameInfo = gameInfo_;
  }
  {
    std::unique_lock<std::mutex> lock(playerMutex_[idx]);
    gameInfo.isMyTurn = playerTurnData_[idx].isTurn();
    gameInfo.gameResult = playerTurnData_[idx].getGameResult();
  }
}

void GameManager::getGameInit(GameInit& gameInit, int32_t playerId) {
  gameInit.gameDescription = gameLogic_.getGameDescription(playerId);
  LOG("GetGameInit %d", playerId);
}

void GameManager::onGameEnd(std::function<void ()> handler) {
  std::unique_lock<std::mutex> lock(gameEndHandlersMutex_);
  gameEndHandlers_.push_back(handler);
}

}}

