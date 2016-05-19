namespace cpp mjollnir.vigridr

struct Coordinate {
  1: required i32 x,
  2: required i32 y
}

struct Command {
  1: required Coordinate coordFrom
  2: required Coordinate coordTo
}