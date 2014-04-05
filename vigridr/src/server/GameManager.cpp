#include "GameManager.h"

#include <stdexcept>

#include "../utils/Log.h"
#include "../utils/MapUtils.h"
#include "GameType.h"
#include "GameConfig.h"

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

GameManager::GameManager(int32_t playerId0, int32_t playerId1) 
  : idToIdx_({{playerId0, 0}, {playerId1, 1}}),
    idxToId_({{playerId0, playerId1}}),
    gameLogic_(playerId0, playerId1) {
  initializeGame(playerId0, playerId1);
  updaterThread_ = std::thread([this]() { 
    updaterTask(); 
  });
}

void GameManager::finalizeGame(bool success) {
  gameInfo_.gameStatus = GameStatus::FINISHED;
  gameInfo_.worldModel = gameLogic_.getWorldModel();
}

void GameManager::initializeGame(int32_t playerId0, int32_t playerId1) {
  std::unique_lock<std::mutex> lock0(playerMutex_[0], std::defer_lock);
  std::unique_lock<std::mutex> lock1(playerMutex_[1], std::defer_lock);
  std::unique_lock<std::mutex> lock2(gameInfoMutex_, std::defer_lock);
  std::lock(lock0, lock1, lock2);
  playerTurnData_[0].init(playerId0);
  playerTurnData_[1].init(playerId1);
  playerTurnData_[0].setIsTurn(true);
  playerTurnData_[1].setIsTurn(true);
  if (config::gameType == GameType::TURN) {
    playerTurnData_[1].setIsTurn(false);
  }
  gameInfo_.cycle = 0;
  gameInfo_.gameStatus = GameStatus::WAITING;
  timer_.startFirstCycle();
}

void GameManager::nextTurn() {
  timer_.sleepUntilWorldModelUpdateTime();
  std::unique_lock<std::mutex> lock0(playerMutex_[0], std::defer_lock);
  std::unique_lock<std::mutex> lock1(playerMutex_[1], std::defer_lock);
  std::unique_lock<std::mutex> lock2(gameInfoMutex_, std::defer_lock);
  std::lock(lock0, lock1, lock2);
  if (config::gameType == GameType::TURN) {
    for (size_t i = 0; i < kMaxPlayers; ++i) {
        playerTurnData_[i].setIsTurn(!playerTurnData_[i].isTurn());   
    } 
  }
  gameInfo_.cycle++;
  gameInfo_.gameStatus = GameStatus::RUNNING;
  gameInfo_.worldModel = gameLogic_.getWorldModel();
  timer_.sleepUntilWorldModelTime();
  timer_.startCycle();
}

bool 
checkCommands(const std::array<PlayerTurnData, kMaxPlayers>& playerTurnData) {
  for (size_t i = 0; i < kMaxPlayers; ++i) {
    if(playerTurnData[i].isTurn() && !playerTurnData[i].isCommandSet()) {
      std::cout << "[ERROR]: No command set for player " << i << std::endl;
      return false;
    } 
  }
  return true;
}

void 
clearCommands(std::array<PlayerTurnData, kMaxPlayers>& playerTurnData) {
  if(playerTurnData[0].isTurn()) { playerTurnData[0].clearCommand(); }
  if(playerTurnData[1].isTurn()) { playerTurnData[1].clearCommand(); }
}

void GameManager::updaterTask() {
  bool errorHappened = true;
  while (true) {
    nextTurn();
    timer_.sleepUntilPlayerUpdateTime();
    std::cout << "Updating..." << std::endl;
    std::array<PlayerTurnData, kMaxPlayers> turns;
    {
      std::unique_lock<std::mutex> lock0(playerMutex_[0], std::defer_lock);
      std::unique_lock<std::mutex> lock1(playerMutex_[1], std::defer_lock);
      std::lock(lock0, lock1);
      errorHappened = !checkCommands(playerTurnData_);
      if (errorHappened) break;
      turns = playerTurnData_;  // copy
      clearCommands(playerTurnData_);
    }
    std::sort(turns.begin(), turns.end(), 
      [](const PlayerTurnData& a, const PlayerTurnData& b) {
        return a.getLastUpdatedTime() < b.getLastUpdatedTime();
    });
    for (auto& turn : turns) {
      if (!turn.isTurn()) { continue; }
      gameLogic_.update(turn.getCommand(), turn.getId()); 
    }
    if (gameLogic_.isFinished()) {
      break;
    }
  }
  finalizeGame(!errorHappened);
}

CommandStatus GameManager::update(const Command& command, int32_t playerId) {
  int32_t idx = utils::getDefault(idToIdx_, playerId, -1);
  CHECK((idx == 0 || idx == 1), "Unknown player with id %d", idx);
  {
    std::unique_lock<std::mutex> lock(playerMutex_[idx]);
    playerTurnData_[idx].setCommand(command, clock::now());
  }
  return CommandStatus::SUCCESS;
}

void GameManager::getGameInfo(GameInfo& gameInfo, int32_t playerId) {
  {
    std::unique_lock<std::mutex> lock(gameInfoMutex_);
    gameInfo = gameInfo_;
  }
  gameInfo.updateTimeLimitMs = timer_.getPlayerUpdateTime();
  gameInfo.nextWorldModelTimeEstimateMs = timer_.getWorldModelTime();
  {
    const size_t kInvalid = 10000;
    size_t idx = utils::getDefault(idToIdx_, playerId, kInvalid);
    CHECK(idx != kInvalid, "Unknown player with id %d.", idx);
    std::unique_lock<std::mutex> lock(playerMutex_[idx]);
    gameInfo.isMyTurn = playerTurnData_[idx].isTurn();
  }
  printf("%d %d\n", 
         gameInfo.updateTimeLimitMs, gameInfo.nextWorldModelTimeEstimateMs);
}  

}}

