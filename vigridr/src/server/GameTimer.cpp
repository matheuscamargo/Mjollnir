#include "GameTimer.h"

#include <thread>

#include "GameConfig.h"

namespace mjollnir { namespace vigridr {

void GameTimer::startCycle() {
  nextUpdateTimeLimit_ = clock::now() + config::cycleDurationMs;
}

void GameTimer::startInitializationCycle() {
  gameStartTime_ = clock::now() + config::firstCycleDurationS;
}

void GameTimer::sleepUntilInitializationTime() {
  if(clock::now() < gameStartTime_) {
    std::this_thread::sleep_until(gameStartTime_);
  }
}

int32_t GameTimer::getTimeUntilGameStart() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
    gameStartTime_-clock::now()).count();
}

int32_t GameTimer::getPlayerUpdateTimeLimit() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
    nextUpdateTimeLimit_-clock::now()).count();
}
}}
