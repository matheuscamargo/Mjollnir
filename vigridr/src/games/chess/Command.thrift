namespace cpp mjollnir.vigridr

struct Coordinate {
  1: required i32 x,
  2: required i32 y
}

enum PromoType {
	PROMOTE_TO_TOWER = 0,
	PROMOTE_TO_BISHOP = 1,
	PROMOTE_TO_HORSE = 2,
	PROMOTE_TO_QUEEN = 3
}

struct Command {
  1: required Coordinate coordFrom,
  2: required Coordinate coordTo,
  3: optional bool smallRock = false,
  4: optional bool bigRock = false,
  5: optional PromoType promoteTo = 3
}