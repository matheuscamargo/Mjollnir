include "WorldModel.thrift"
include "GameDescription.thrift"
include "GameResult.thrift"

namespace cpp mjollnir.vigridr

enum GameStatus {
  RUNNING = 1,
  WAITING = 2,
  FINISHED = 3
}

enum CommandStatus {
  SUCCESS = 1,
  ERROR = 2
}

struct GameInfo {
  1: required GameStatus gameStatus,
  2: required WorldModel.WorldModel worldModel,
  3: required i32 cycle,
  4: required bool isMyTurn,
  5: required GameResult.GameResult gameResult
}

struct GameInit {
  1: required GameInfo gameInfo,
  2: required GameDescription.GameDescription gameDescription,
}
