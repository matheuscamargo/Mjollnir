namespace cpp mjollnir.vigridr

enum Piece {
	PAWN = 1,
	TOWER = 2,
	HORSE = 3,
	BISHOP = 4,
	QUEEN = 5,
	KING = 6,
	EMPTY = 7
}

struct WorldModel {
  1: required list<list<Piece>> board
}

