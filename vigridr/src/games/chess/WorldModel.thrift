namespace cpp mjollnir.vigridr

enum Type {
	PAWN = 1,
	TOWER = 2,
	HORSE = 3,
	BISHOP = 4,
	QUEEN = 5,
	KING = 6,
	EMPTY = 7
}

enum PlayerColor {
  BLACK = 0,
  WHITE = 1
}

struct Piece {
	1: required Type type,
	2: optional PlayerColor owner
}

struct WorldModel {
  1: required list<list<Piece>> board
}

