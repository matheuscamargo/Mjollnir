#include "GameTimer.h"

#include <thread>

#include "GameConfig.h"

namespace mjollnir { namespace vigridr {

/**
 *  Constants to synchronize time updating so that the new world model
 *  is available exactly when promised.
 */
const std::chrono::milliseconds kWorldModelUpdateMsStep1(2);
const std::chrono::milliseconds kWorldModelUpdateMsStep2(1);

void GameTimer::startCycle() {
  nextUpdateTimeLimit_ = clock::now() + config::cycleDurationMs;
}

void GameTimer::startInitializationCycle() {
  nextUpdateTimeLimit_ = clock::now() + config::firstCycleDurationS;
  nextWorldModelTime_ = nextUpdateTimeLimit_ + config::updateTimeUpperBoundMs;
}

void GameTimer::sleepUntilInitializationUpdateTime() {
  if(clock::now() < nextWorldModelTime_) {
    std::this_thread::sleep_until(nextWorldModelTime_ - kWorldModelUpdateMsStep1);
  }
}

void GameTimer::sleepUntilInitializationTime() {
  if(clock::now() < nextWorldModelTime_) {
    std::this_thread::sleep_until(nextWorldModelTime_);
  }
}

int32_t GameTimer:: getWorldModelTime() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
    nextWorldModelTime_-clock::now()).count();
}

int32_t GameTimer::getPlayerUpdateTimeLimit() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
    nextUpdateTimeLimit_-clock::now()).count();
}
}}
