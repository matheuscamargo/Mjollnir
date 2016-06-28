#include <fstream>

#include "../server/GameLogic.h"
#include "gtest/gtest.h"


namespace mjollnir { namespace vigridr {

class GameLogicTest : public ::testing::Test {
 protected:

  GameLogicTest() : game1(std::vector<int32_t>{9090, 9091}) { 
    game1.setFirstPlayer(9090);
    game1.setHasFinished(false);
    game1.setWinner("-1");
  }

  // void CleanTable() {
  //   Coordinate coord;
  //   for(size_t i = 0; i < 3; ++i) {
  //     for(size_t j = 0; j < 3; ++j) {
  //       coord.x = i; coord.y = j;
  //       game1.setTableCoordinate(coord, Marker::UNMARKED);
  //     }
  //   }
  //   game1.setHasFinished(false);
  //   game1.setWinner("-1");
  // }

  Type getTypeOfPieceAtColumn(int column){
    switch(column){
      case 0:
      case 7:
        return Type::TOWER;
      case 1:
      case 6:
        return Type::HORSE;
      case 2:
      case 5:
        return Type::BISHOP;
      case 3:
        return Type::QUEEN;
      case 4:
        return Type::KING;
      default:
        return Type::EMPTY;
    }
  }

  std::vector<std::vector<Piece> > constructInitialBoard(){
    Piece newPiece;

    std::vector<std::vector<Piece> > initialBoard;
    for (int i = 0; i < 8; i++) {
      std::vector<Piece> line;
      for (int j = 0; j < 8; j++) {
        newPiece.type = Type::EMPTY;
        line.push_back(newPiece);
      }
      initialBoard.push_back(line);
    }

    for (int i = 0; i < 8; i++) {
      for(int j = 0; j < 8; j++){
        switch(i){
          case 0:
            newPiece.owner = PlayerColor::BLACK;
            newPiece.type = getTypeOfPieceAtColumn(j);
            initialBoard[i][j] = newPiece;
            break;
          case 1:
            newPiece.owner = PlayerColor::BLACK;
            newPiece.type = Type::PAWN;
            initialBoard[i][j] = newPiece;
            break;
          case 6:
            newPiece.owner = PlayerColor::WHITE;
            newPiece.type = Type::PAWN;
            initialBoard[i][j] = newPiece;
            break;
          case 7:
            newPiece.owner = PlayerColor::WHITE;
            newPiece.type = getTypeOfPieceAtColumn(j);
            initialBoard[i][j] = newPiece;
            break;
          default:
            break;
        }
      }
    }

    return initialBoard;
  }

  std::vector<std::vector<Piece> > constructEmptyBoard(){
    Piece newPiece;

    std::vector<std::vector<Piece> > emptyBoard;
    for (int i = 0; i < 8; i++) {
      std::vector<Piece> line;
      for (int j = 0; j < 8; j++) {
        newPiece.type = Type::EMPTY;
        line.push_back(newPiece);
      }
      emptyBoard.push_back(line);
    }

    return emptyBoard;
  }

  Command createCommand(int32_t x0, int32_t y0, int32_t x1, int32_t y1){
    Command c;
    c.coordFrom.x = x0;
    c.coordFrom.y = y0;
    c.coordTo.x = x1;
    c.coordTo.y = y1;
    return c;
  }

  Command createSmallRockCommand(){
    Command c;
    c.smallRock = true;
    return c;
  }

  Command createBigRockCommand(){
    Command c;
    c.bigRock = true;
    return c;
  }

  Piece createEmptyPiece(){
    Piece emptyPiece;
    emptyPiece.type = Type::EMPTY;
    return emptyPiece;
  }

  Piece createPiece(Type type, PlayerColor color){
    Piece newPiece;
    newPiece.type = type;
    newPiece.owner = color;
    return newPiece;
  }

  char toChar(Piece p) {
    if (p.owner == PlayerColor::BLACK) {
      switch(p.type) {
        case Type::PAWN: return 'P';
        case Type::TOWER: return 'T';
        case Type::HORSE: return 'H';
        case Type::BISHOP: return 'B';
        case Type::QUEEN: return 'Q';
        case Type::KING: return 'K';
        default: return '-';
      }
    }
    else {    
      switch(p.type) {
        case Type::PAWN: return 'p';
        case Type::TOWER: return 't';
        case Type::HORSE: return 'h';
        case Type::BISHOP: return 'b';
        case Type::QUEEN: return 'q';
        case Type::KING: return 'k';
        default: return '-';
      }
    }
  }

  void printBoard(const std::vector<std::vector<Piece> >& board) {
    std::ostringstream oss;

    oss << "    ╔═══════════════╗" << std::endl;

    for (int i=0; i<8; i++) {

      if( i == 3 )
        oss << 'x';
      else
        oss << ' ';
      oss << ' ' << i << " ║";

      for (int j=0; j<8; j++) {
        oss << toChar(board[i][j]);
        if( j != 8-1 ){
          oss << ' ';
        }
      }
      oss << "║" << std::endl;
    }

    oss << "    ╚═══════════════╝" << std::endl;
    oss << "     0 1 2 3 4 5 6 7 " << std::endl;
    oss << "            y        " << std::endl;

    std::cout << oss.str();
  }

  bool containSmallRock(std::vector<Command>& moveList){
    for( unsigned int i = 0; i < moveList.size(); i++ ){
      if(moveList[i].smallRock == true){
        return true;
      }
    }
    return false;
  }

  bool containBigRock(std::vector<Command>& moveList){
    for( unsigned int i = 0; i < moveList.size(); i++ ){
      if(moveList[i].bigRock == true){
        return true;
      }
    }
    return false;
  }

  bool containCommand(std::vector<Command>& moveList, Command command){
    for( unsigned int i = 0; i < moveList.size(); i++ ){
      if(moveList[i] == command){
        return true;
      }
    }
    return false;
  }

  GameLogic game1;
};

TEST_F(GameLogicTest, TestingIntialPositionOfPieces) {
  std::vector<std::vector<Piece> > expBoard = constructInitialBoard();

  ASSERT_EQ(expBoard, game1.getWorldModel().board);
}

TEST_F(GameLogicTest, TestingWhiteSmallRock) {
  std::vector<std::vector<Piece> > expBoard = constructInitialBoard();

  expBoard[1][0] = createEmptyPiece();
  expBoard[1][1] = createEmptyPiece();
  expBoard[1][2] = createEmptyPiece();
  expBoard[1][3] = createEmptyPiece();
  expBoard[2][0] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[2][1] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[2][2] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[2][3] = createPiece(Type::PAWN, PlayerColor::BLACK);

  expBoard[7][4] = createEmptyPiece();
  expBoard[7][5] = createPiece(Type::TOWER, PlayerColor::WHITE);
  expBoard[7][6] = createPiece(Type::KING, PlayerColor::WHITE);
  expBoard[7][7] = createEmptyPiece();
  expBoard[6][6] = createEmptyPiece();
  expBoard[5][5] = createPiece(Type::HORSE, PlayerColor::WHITE);
  expBoard[5][6] = createPiece(Type::PAWN, PlayerColor::WHITE);
  expBoard[5][7] = createPiece(Type::BISHOP, PlayerColor::WHITE);

  EXPECT_TRUE(game1.update(createCommand(6, 6, 5, 6), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 0, 2, 0), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 6, 5, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 1, 2, 1), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 5, 5, 7), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 2, 2, 2), 9091));
  EXPECT_TRUE(game1.update(createSmallRockCommand(), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 3, 2, 3), 9091));
  expBoard[2][3].moved = true;

  ASSERT_EQ(expBoard, game1.getWorldModel().board);
}

TEST_F(GameLogicTest, TestingWhiteSmallRockInMoveList) {
  EXPECT_TRUE(game1.update(createCommand(6, 6, 5, 6), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 0, 2, 0), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 6, 5, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 1, 2, 1), 9091));
  EXPECT_FALSE(containSmallRock(game1.getMoveList(9090)));
  EXPECT_TRUE(game1.update(createCommand(7, 5, 5, 7), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 2, 2, 2), 9091));
  EXPECT_TRUE(containSmallRock(game1.getMoveList(9090)));
}

TEST_F(GameLogicTest, TestingWhiteSmallRockFailWhenPieceBetweenKingAndTower) {
  EXPECT_TRUE(game1.update(createCommand(6, 6, 5, 6), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 0, 2, 0), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 6, 5, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 1, 2, 1), 9091));
  EXPECT_FALSE(game1.update(createSmallRockCommand(), 9090));
}

TEST_F(GameLogicTest, TestingWhiteSmallRockFailWhenKingAlreadyMoved) {
  EXPECT_TRUE(game1.update(createCommand(6, 6, 5, 6), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 0, 2, 0), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 6, 5, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 1, 2, 1), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 5, 5, 7), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 2, 2, 2), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 4, 7, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 3, 2, 3), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 5, 7, 4), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 4, 2, 4), 9091));
  EXPECT_FALSE(game1.update(createSmallRockCommand(), 9090));
}

TEST_F(GameLogicTest, TestingWhiteSmallRockFailWhenTowerAlreadyMoved) {
  EXPECT_TRUE(game1.update(createCommand(6, 6, 5, 6), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 0, 2, 0), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 6, 5, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 1, 2, 1), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 5, 5, 7), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 2, 2, 2), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 7, 7, 6), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 3, 2, 3), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 6, 7, 7), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 4, 2, 4), 9091));
  EXPECT_FALSE(game1.update(createSmallRockCommand(), 9090));
}

TEST_F(GameLogicTest, TestingWhiteSmallRockFailWhenKingOnCheck) {
  EXPECT_TRUE(game1.update(createCommand(6, 5, 5, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 4, 2, 4), 9091));
  EXPECT_TRUE(game1.update(createCommand(6, 4, 5, 4), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 5, 1, 4), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 5, 5, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 0, 2, 0), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 6, 5, 7), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 4, 4, 7), 9091));
  EXPECT_FALSE(game1.update(createSmallRockCommand(), 9090));
}

TEST_F(GameLogicTest, TestingWhiteSmallRockFailWhenKingOnCheckAfterRock) {
  EXPECT_TRUE(game1.update(createCommand(6, 6, 4, 6), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 7, 3, 7), 9091));

  EXPECT_TRUE(game1.update(createCommand(4, 6, 3, 7), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 7, 3, 7), 9091));

  EXPECT_TRUE(game1.update(createCommand(7, 6, 5, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(3, 7, 3, 6), 9091));

  EXPECT_TRUE(game1.update(createCommand(7, 5, 5, 7), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 0, 2, 0), 9091));

  EXPECT_FALSE(game1.update(createSmallRockCommand(), 9090));
}

TEST_F(GameLogicTest, TestingWhiteBigRock) {
  std::vector<std::vector<Piece> > expBoard = constructInitialBoard();

  expBoard[1][0] = createEmptyPiece();
  expBoard[1][1] = createEmptyPiece();
  expBoard[1][2] = createEmptyPiece();
  expBoard[1][3] = createEmptyPiece();
  expBoard[1][4] = createEmptyPiece();
  expBoard[2][0] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[2][1] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[2][2] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[2][3] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[2][4] = createPiece(Type::PAWN, PlayerColor::BLACK);

  expBoard[7][0] = createEmptyPiece();
  expBoard[7][1] = createEmptyPiece();
  expBoard[7][2] = createPiece(Type::KING, PlayerColor::WHITE);
  expBoard[7][3] = createPiece(Type::TOWER, PlayerColor::WHITE);
  expBoard[7][4] = createEmptyPiece();
  expBoard[6][3] = createEmptyPiece();
  expBoard[5][0] = createPiece(Type::HORSE, PlayerColor::WHITE);
  expBoard[5][3] = createPiece(Type::QUEEN, PlayerColor::WHITE);
  expBoard[5][4] = createPiece(Type::BISHOP, PlayerColor::WHITE);
  expBoard[4][3] = createPiece(Type::PAWN, PlayerColor::WHITE);

  EXPECT_TRUE(game1.update(createCommand(6, 3, 4, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 0, 2, 0), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 1, 5, 0), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 1, 2, 1), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 2, 5, 4), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 2, 2, 2), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 3, 5, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 3, 2, 3), 9091));
  EXPECT_TRUE(game1.update(createBigRockCommand(), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 4, 2, 4), 9091));
  expBoard[2][4].moved = true;

  ASSERT_EQ(expBoard, game1.getWorldModel().board);
}

TEST_F(GameLogicTest, TestingWhiteBigRockInMoveList) {
  EXPECT_TRUE(game1.update(createCommand(6, 3, 4, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 0, 2, 0), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 1, 5, 0), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 1, 2, 1), 9091));
  EXPECT_TRUE(game1.update(createCommand(7, 2, 5, 4), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 2, 2, 2), 9091));
  EXPECT_FALSE(containBigRock(game1.getMoveList(9090)));
  EXPECT_TRUE(game1.update(createCommand(7, 3, 5, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 3, 2, 3), 9091));
  EXPECT_TRUE(containBigRock(game1.getMoveList(9090)));
  EXPECT_TRUE(game1.update(createBigRockCommand(), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 4, 2, 4), 9091));
}

TEST_F(GameLogicTest, TestingBlackSmallRock) {
  std::vector<std::vector<Piece> > expBoard = constructInitialBoard();

  expBoard[0][4] = createEmptyPiece();
  expBoard[0][5] = createPiece(Type::TOWER, PlayerColor::BLACK);
  expBoard[0][6] = createPiece(Type::KING, PlayerColor::BLACK);
  expBoard[0][7] = createEmptyPiece();
  expBoard[1][6] = createEmptyPiece();
  expBoard[2][5] = createPiece(Type::HORSE, PlayerColor::BLACK);
  expBoard[2][6] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[2][7] = createPiece(Type::BISHOP, PlayerColor::BLACK);

  expBoard[6][0] = createEmptyPiece();
  expBoard[6][1] = createEmptyPiece();
  expBoard[6][2] = createEmptyPiece();
  expBoard[6][3] = createEmptyPiece();
  expBoard[6][4] = createEmptyPiece();
  expBoard[5][0] = createPiece(Type::PAWN, PlayerColor::WHITE);
  expBoard[5][1] = createPiece(Type::PAWN, PlayerColor::WHITE);
  expBoard[5][2] = createPiece(Type::PAWN, PlayerColor::WHITE);
  expBoard[5][3] = createPiece(Type::PAWN, PlayerColor::WHITE);
  expBoard[5][4] = createPiece(Type::PAWN, PlayerColor::WHITE);


  EXPECT_TRUE(game1.update(createCommand(6, 0, 5, 0), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 6, 2, 6), 9091));
  EXPECT_TRUE(game1.update(createCommand(6, 1, 5, 1), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 5, 2, 7), 9091));
  EXPECT_TRUE(game1.update(createCommand(6, 2, 5, 2), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 6, 2, 5), 9091));
  EXPECT_TRUE(game1.update(createCommand(6, 3, 5, 3), 9090));
  EXPECT_TRUE(game1.update(createSmallRockCommand(), 9091));
  EXPECT_TRUE(game1.update(createCommand(6, 4, 5, 4), 9090));
  expBoard[5][4].moved = true;

  ASSERT_EQ(expBoard, game1.getWorldModel().board);
}

TEST_F(GameLogicTest, TestingBlackSmallRockInMoveList) {
  EXPECT_TRUE(game1.update(createCommand(6, 0, 5, 0), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 6, 2, 6), 9091));
  EXPECT_TRUE(game1.update(createCommand(6, 1, 5, 1), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 5, 2, 7), 9091));
  EXPECT_TRUE(game1.update(createCommand(6, 2, 5, 2), 9090));
  EXPECT_FALSE(containSmallRock(game1.getMoveList(9091)));
  EXPECT_TRUE(game1.update(createCommand(0, 6, 2, 5), 9091));
  EXPECT_TRUE(game1.update(createCommand(6, 3, 5, 3), 9090));
  EXPECT_TRUE(containSmallRock(game1.getMoveList(9091)));
  EXPECT_TRUE(game1.update(createSmallRockCommand(), 9091));
  EXPECT_TRUE(game1.update(createCommand(6, 4, 5, 4), 9090));
}

TEST_F(GameLogicTest, TestingBlackBigRock) {
  std::vector<std::vector<Piece> > expBoard = constructInitialBoard();

  expBoard[0][0] = createEmptyPiece();
  expBoard[0][1] = createEmptyPiece();
  expBoard[0][2] = createPiece(Type::KING, PlayerColor::BLACK);
  expBoard[0][3] = createPiece(Type::TOWER, PlayerColor::BLACK);
  expBoard[0][4] = createEmptyPiece();
  expBoard[1][3] = createEmptyPiece();
  expBoard[2][2] = createPiece(Type::HORSE, PlayerColor::BLACK);
  expBoard[2][3] = createPiece(Type::QUEEN, PlayerColor::BLACK);
  expBoard[2][4] = createPiece(Type::BISHOP, PlayerColor::BLACK);
  expBoard[3][3] = createPiece(Type::PAWN, PlayerColor::BLACK);

  expBoard[6][0] = createEmptyPiece();
  expBoard[6][1] = createEmptyPiece();
  expBoard[6][2] = createEmptyPiece();
  expBoard[6][3] = createEmptyPiece();
  expBoard[6][4] = createEmptyPiece();
  expBoard[6][5] = createEmptyPiece();
  expBoard[5][0] = createPiece(Type::PAWN, PlayerColor::WHITE);
  expBoard[5][1] = createPiece(Type::PAWN, PlayerColor::WHITE);
  expBoard[5][2] = createPiece(Type::PAWN, PlayerColor::WHITE);
  expBoard[5][3] = createPiece(Type::PAWN, PlayerColor::WHITE);
  expBoard[5][4] = createPiece(Type::PAWN, PlayerColor::WHITE);
  expBoard[5][5] = createPiece(Type::PAWN, PlayerColor::WHITE);

  EXPECT_TRUE(game1.update(createCommand(6, 0, 5, 0), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 3, 3, 3), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 1, 5, 1), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 2, 2, 4), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 2, 5, 2), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 3, 2, 3), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 3, 5, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 1, 2, 2), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 4, 5, 4), 9090));
  EXPECT_TRUE(game1.update(createBigRockCommand(), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 5, 5, 5), 9090));
  expBoard[5][5].moved = true;

  ASSERT_EQ(expBoard, game1.getWorldModel().board);
}

TEST_F(GameLogicTest, TestingBlackBigRockInMoveList) {
  EXPECT_TRUE(game1.update(createCommand(6, 0, 5, 0), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 3, 3, 3), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 1, 5, 1), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 2, 2, 4), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 2, 5, 2), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 3, 2, 3), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 3, 5, 3), 9090));
  EXPECT_FALSE(containBigRock(game1.getMoveList(9091)));
  EXPECT_TRUE(game1.update(createCommand(0, 1, 2, 2), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 4, 5, 4), 9090));
  EXPECT_TRUE(containBigRock(game1.getMoveList(9091)));
  EXPECT_TRUE(game1.update(createBigRockCommand(), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 5, 5, 5), 9090));
}

// The game is between Adolf Anderssen vs Lionel Adalbert Bagration Felix Kieseritzky (1851)
// It's available at: <http://www.chessgames.com/perl/chessgame?gid=1018910>
TEST_F(GameLogicTest, TestingFullGame) { 
  std::vector<std::vector<Piece> > expBoard = constructEmptyBoard();

  expBoard[0][0] = createPiece(Type::TOWER, PlayerColor::BLACK);
  expBoard[0][2] = createPiece(Type::BISHOP, PlayerColor::BLACK);
  expBoard[0][3] = createPiece(Type::KING, PlayerColor::BLACK);
  expBoard[0][7] = createPiece(Type::TOWER, PlayerColor::BLACK);

  expBoard[1][0] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[1][3] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[1][5] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[1][7] = createPiece(Type::PAWN, PlayerColor::BLACK);

  expBoard[2][0] = createPiece(Type::HORSE, PlayerColor::BLACK);
  expBoard[2][5] = createPiece(Type::HORSE, PlayerColor::BLACK);

  expBoard[3][1] = createPiece(Type::PAWN, PlayerColor::BLACK);

  expBoard[7][0] = createPiece(Type::QUEEN, PlayerColor::BLACK);
  expBoard[7][6] = createPiece(Type::BISHOP, PlayerColor::BLACK);

  expBoard[1][4] = createPiece(Type::BISHOP, PlayerColor::WHITE);
  expBoard[1][6] = createPiece(Type::HORSE, PlayerColor::WHITE);

  expBoard[3][3] = createPiece(Type::HORSE, PlayerColor::WHITE);
  expBoard[3][4] = createPiece(Type::PAWN, PlayerColor::WHITE);
  expBoard[3][7] = createPiece(Type::PAWN, PlayerColor::WHITE);

  expBoard[4][6] = createPiece(Type::PAWN, PlayerColor::WHITE);

  expBoard[5][3] = createPiece(Type::PAWN, PlayerColor::WHITE);

  expBoard[6][0] = createPiece(Type::PAWN, PlayerColor::WHITE);
  expBoard[6][2] = createPiece(Type::PAWN, PlayerColor::WHITE);
  expBoard[6][4] = createPiece(Type::KING, PlayerColor::WHITE);

  //1
  EXPECT_TRUE(game1.update(createCommand(6, 4, 4, 4), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 4, 3, 4), 9091));

  //2
  EXPECT_TRUE(game1.update(createCommand(6, 5, 4, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(3, 4, 4, 5), 9091));

  //3
  EXPECT_TRUE(game1.update(createCommand(7, 5, 4, 2), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 3, 4, 7), 9091));

  //4
  EXPECT_TRUE(game1.update(createCommand(7, 4, 7, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 1, 3, 1), 9091));

  //5
  EXPECT_TRUE(game1.update(createCommand(4, 2, 3, 1), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 6, 2, 5), 9091));

  //6
  EXPECT_TRUE(game1.update(createCommand(7, 6, 5, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(4, 7, 2, 7), 9091));

  //7
  EXPECT_TRUE(game1.update(createCommand(6, 3, 5, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(2, 5, 3, 7), 9091));

  //8
  EXPECT_TRUE(game1.update(createCommand(5, 5, 4, 7), 9090));
  EXPECT_TRUE(game1.update(createCommand(2, 7, 3, 6), 9091));

  //9
  EXPECT_TRUE(game1.update(createCommand(4, 7, 3, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 2, 2, 2), 9091));

  //10
  EXPECT_TRUE(game1.update(createCommand(6, 6, 4, 6), 9090));
  EXPECT_TRUE(game1.update(createCommand(3, 7, 2, 5), 9091));

  //11
  EXPECT_TRUE(game1.update(createCommand(7, 7, 7, 6), 9090));
  EXPECT_TRUE(game1.update(createCommand(2, 2, 3, 1), 9091));

  //12
  EXPECT_TRUE(game1.update(createCommand(6, 7, 4, 7), 9090));
  EXPECT_TRUE(game1.update(createCommand(3, 6, 2, 6), 9091));

  //13
  EXPECT_TRUE(game1.update(createCommand(4, 7, 3, 7), 9090));
  EXPECT_TRUE(game1.update(createCommand(2, 6, 3, 6), 9091));

  //14
  EXPECT_TRUE(game1.update(createCommand(7, 3, 5, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(2, 5, 0, 6), 9091));

  //15
  EXPECT_TRUE(game1.update(createCommand(7, 2, 4, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(3, 6, 2, 5), 9091));

  //16
  EXPECT_TRUE(game1.update(createCommand(7, 1, 5, 2), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 5, 3, 2), 9091));

  //17
  EXPECT_TRUE(game1.update(createCommand(5, 2, 3, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(2, 5, 6, 1), 9091));

  //18
  EXPECT_TRUE(game1.update(createCommand(4, 5, 2, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(3, 2, 7, 6), 9091));

  //19
  EXPECT_TRUE(game1.update(createCommand(4, 4, 3, 4), 9090));
  EXPECT_TRUE(game1.update(createCommand(6, 1, 7, 0), 9091));

  //20
  EXPECT_TRUE(game1.update(createCommand(7, 5, 6, 4), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 1, 2, 0), 9091));

  //21
  EXPECT_TRUE(game1.update(createCommand(3, 5, 1, 6), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 4, 0, 3), 9091));

  //22
  EXPECT_TRUE(game1.update(createCommand(5, 5, 2, 5), 9090));
  EXPECT_TRUE(game1.update(createCommand(0, 6, 2, 5), 9091));

  //23
  EXPECT_TRUE(game1.update(createCommand(2, 3, 1, 4), 9090));
  expBoard[1][4].moved = true;

  // TODO: Check if the game was finished

  // printBoard(expBoard);
  // printBoard(game1.getWorldModel().board);

  ASSERT_EQ(expBoard, game1.getWorldModel().board);
}

TEST_F(GameLogicTest, TestingWhiteMakingEnPassant) {
  std::vector<std::vector<Piece> > expBoard = constructInitialBoard();

  expBoard[1][4] = createEmptyPiece();
  expBoard[1][7] = createEmptyPiece();
  expBoard[2][7] = createPiece(Type::PAWN, PlayerColor::BLACK);

  expBoard[6][3] = createEmptyPiece();
  expBoard[2][4] = createPiece(Type::PAWN, PlayerColor::WHITE);

  EXPECT_TRUE(game1.update(createCommand(6, 3, 4, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 7, 2, 7), 9091));

  EXPECT_TRUE(game1.update(createCommand(4, 3, 3, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 4, 3, 4), 9091));

  EXPECT_TRUE(game1.update(createCommand(3, 3, 2, 4), 9090));
  expBoard[2][4].moved = true;

  // printBoard(expBoard);
  // printBoard(game1.getWorldModel().board);

  ASSERT_EQ(expBoard, game1.getWorldModel().board);
}

TEST_F(GameLogicTest, TestingWhiteEnPassantInMoveList) {
  EXPECT_TRUE(game1.update(createCommand(6, 3, 4, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 7, 2, 7), 9091));

  EXPECT_TRUE(game1.update(createCommand(4, 3, 3, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 4, 3, 4), 9091));
  EXPECT_TRUE(containCommand(game1.getMoveList(9090), createCommand(3,3,2,4)));

  EXPECT_TRUE(game1.update(createCommand(3, 3, 2, 4), 9090));
}

TEST_F(GameLogicTest, TestingWhiteFailingEnPassant) {
  EXPECT_TRUE(game1.update(createCommand(6, 3, 4, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 7, 2, 7), 9091));

  EXPECT_TRUE(game1.update(createCommand(4, 3, 3, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 4, 3, 4), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 0, 5, 0), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 0, 2, 0), 9091));

  EXPECT_FALSE(game1.update(createCommand(3, 3, 2, 4), 9090));
}

TEST_F(GameLogicTest, TestingBlackMakingEnPassant) {
  std::vector<std::vector<Piece> > expBoard = constructInitialBoard();

  expBoard[1][3] = createEmptyPiece();
  expBoard[5][2] = createPiece(Type::PAWN, PlayerColor::BLACK);

  expBoard[6][0] = createEmptyPiece();
  expBoard[6][1] = createEmptyPiece();
  expBoard[6][2] = createEmptyPiece();
  expBoard[5][0] = createPiece(Type::PAWN, PlayerColor::WHITE);
  expBoard[5][1] = createPiece(Type::PAWN, PlayerColor::WHITE);

  EXPECT_TRUE(game1.update(createCommand(6, 0, 5, 0), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 3, 3, 3), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 1, 5, 1), 9090));
  EXPECT_TRUE(game1.update(createCommand(3, 3, 4, 3), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 2, 4, 2), 9090));
  EXPECT_TRUE(game1.update(createCommand(4, 3, 5, 2), 9091));
  expBoard[5][2].moved = true;

  // printBoard(expBoard);
  // printBoard(game1.getWorldModel().board);

  ASSERT_EQ(expBoard, game1.getWorldModel().board);
}

TEST_F(GameLogicTest, TestingBlackEnPassantInMoveList) {
  EXPECT_TRUE(game1.update(createCommand(6, 0, 5, 0), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 3, 3, 3), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 1, 5, 1), 9090));
  EXPECT_TRUE(game1.update(createCommand(3, 3, 4, 3), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 2, 4, 2), 9090));
  EXPECT_TRUE(containCommand(game1.getMoveList(9091), createCommand(4, 3, 5, 2)));
  EXPECT_TRUE(game1.update(createCommand(4, 3, 5, 2), 9091));
}

TEST_F(GameLogicTest, TestingBlackFailingEnPassant) {
  EXPECT_TRUE(game1.update(createCommand(6, 0, 5, 0), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 3, 3, 3), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 1, 5, 1), 9090));
  EXPECT_TRUE(game1.update(createCommand(3, 3, 4, 3), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 2, 4, 2), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 0, 2, 0), 9091));

  EXPECT_TRUE(game1.update(createCommand(6, 7, 5, 7), 9090));
  EXPECT_FALSE(game1.update(createCommand(4, 3, 5, 2), 9091));
}

TEST_F(GameLogicTest, TestingWhiteMakingPawnPromotion) {
  std::vector<std::vector<Piece> > expBoard = constructInitialBoard();

  expBoard[1][2] = createEmptyPiece();
  expBoard[1][5] = createEmptyPiece();
  expBoard[1][6] = createEmptyPiece();
  expBoard[1][7] = createEmptyPiece();

  expBoard[2][5] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[2][6] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[2][7] = createPiece(Type::PAWN, PlayerColor::BLACK);

  expBoard[6][3] = createEmptyPiece();

  expBoard[0][1] = createPiece(Type::HORSE, PlayerColor::WHITE);

  EXPECT_TRUE(game1.update(createCommand(6, 3, 4, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 7, 2, 7), 9091));

  EXPECT_TRUE(game1.update(createCommand(4, 3, 3, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 2, 3, 2), 9091));

  EXPECT_TRUE(game1.update(createCommand(3, 3, 2, 2), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 6, 2, 6), 9091));

  EXPECT_TRUE(game1.update(createCommand(2, 2, 1, 2), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 5, 2, 5), 9091));

  Command c = createCommand(1, 2, 0, 1);
  c.promoteTo = PromoType::PROMOTE_TO_HORSE;
  EXPECT_TRUE(game1.update(c, 9090));
  expBoard[0][1].moved = true;

  // printBoard(expBoard);
  // printBoard(game1.getWorldModel().board);

  ASSERT_EQ(expBoard, game1.getWorldModel().board);
}

TEST_F(GameLogicTest, TestingWhitePawnPromotionInMoveList) {
  std::vector<std::vector<Piece> > expBoard = constructInitialBoard();

  expBoard[1][2] = createEmptyPiece();
  expBoard[1][5] = createEmptyPiece();
  expBoard[1][6] = createEmptyPiece();
  expBoard[1][7] = createEmptyPiece();

  expBoard[2][5] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[2][6] = createPiece(Type::PAWN, PlayerColor::BLACK);
  expBoard[2][7] = createPiece(Type::PAWN, PlayerColor::BLACK);

  expBoard[6][3] = createEmptyPiece();

  expBoard[0][1] = createPiece(Type::HORSE, PlayerColor::WHITE);

  EXPECT_TRUE(game1.update(createCommand(6, 3, 4, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 7, 2, 7), 9091));

  EXPECT_TRUE(game1.update(createCommand(4, 3, 3, 3), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 2, 3, 2), 9091));

  EXPECT_TRUE(game1.update(createCommand(3, 3, 2, 2), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 6, 2, 6), 9091));

  EXPECT_TRUE(game1.update(createCommand(2, 2, 1, 2), 9090));
  EXPECT_TRUE(game1.update(createCommand(1, 5, 2, 5), 9091));


  Command c;
  c = createCommand(1, 2, 0, 1);
  
  // TODO: Add different promotion pieces in moveList
  // c.promoteTo = PromoType::PROMOTE_TO_HORSE;
  // EXPECT_TRUE(containCommand(game1.getMoveList(9090), c));

  // c.promoteTo = PromoType::PROMOTE_TO_TOWER;
  // EXPECT_TRUE(containCommand(game1.getMoveList(9090), c));

  // c.promoteTo = PromoType::PROMOTE_TO_BISHOP;
  // EXPECT_TRUE(containCommand(game1.getMoveList(9090), c));

  c.promoteTo = PromoType::PROMOTE_TO_QUEEN;
  EXPECT_TRUE(containCommand(game1.getMoveList(9090), c));

  c = createCommand(1, 2, 0, 1);
  c.promoteTo = PromoType::PROMOTE_TO_HORSE;
  EXPECT_TRUE(game1.update(c, 9090));
  expBoard[0][1].moved = true;

  // printBoard(expBoard);
  // printBoard(game1.getWorldModel().board);

  ASSERT_EQ(expBoard, game1.getWorldModel().board);
}

}}  // namespaces

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}