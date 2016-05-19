#ifndef VIGRIDR_CLIENT_CLIENT_LOGIC_H
#define VIGRIDR_CLIENT_CLIENT_LOGIC_H

#include "../thrifts/gen-cpp/Game.h"

void init(const mjollnir::vigridr::GameInit& gameInit);

mjollnir::vigridr::Command playTurn(const mjollnir::vigridr::WorldModel& wm, int32_t turn);

void endOfGame(const mjollnir::vigridr::GameResult& result);

#endif  // VIGRIDR_CLIENT_CLIENT_LOGIC_H

