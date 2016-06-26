namespace cpp mjollnir.vigridr

struct Coordinate {
  1: required i32 x,
  2: required i32 y
}

struct Command {
  1: required Coordinate coordFrom,
  2: required Coordinate coordTo,
  3: optional bool smallRock = false,
  4: optional bool bigRock = false
}