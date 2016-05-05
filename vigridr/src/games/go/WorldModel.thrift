namespace cpp mjollnir.vigridr

enum Marker {
  UNMARKED = 1,
  X = 2,
  O = 3
}

struct WorldModel {
  1: list<list<Marker>> table;
}