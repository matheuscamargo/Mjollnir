#ifndef VIGRIDR_SERVER_GAME_TIMER_H
#define VIGRIDR_SERVER_GAME_TIMER_H

#include <chrono>

namespace mjollnir { namespace vigridr {

typedef std::chrono::high_resolution_clock clock;
typedef std::chrono::time_point<clock> time_point;

class GameTimer {
 public:
  void startInitializationCycle();
  void startCycle();
  void sleepUntilInitializationTime();
  int32_t getTimeUntilGameStart();
  int32_t getPlayerUpdateTimeLimit();


 private:
  time_point nextUpdateTimeLimit_;
  time_point gameStartTime_;
};


}}

#endif  // VIGRIDR_SERVER_GAME_TIMER_H