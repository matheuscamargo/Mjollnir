#ifndef VIGRIDR_CLIENT_CLIENT_LOGIC_H
#define VIGRIDR_CLIENT_CLIENT_LOGIC_H

#include "../thrifts/gen-cpp/Game.h"

mjollnir::vigridr::Command playTurn(const mjollnir::vigridr::WorldModel& wm,
  const std::vector<mjollnir::vigridr::Command>& moveList, int32_t turn);

void init(const mjollnir::vigridr::GameInit& gameInit);

void endOfGame(const mjollnir::vigridr::GameResult& result);

#endif  // VIGRIDR_CLIENT_CLIENT_LOGIC_H
