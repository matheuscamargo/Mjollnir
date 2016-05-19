include "WorldModel.thrift"

namespace cpp mjollnir.vigridr

enum PlayerColor {
  BLACK = 0,
  WHITE = 1
}

struct GameDescription {
  1: required PlayerColor myColor
}