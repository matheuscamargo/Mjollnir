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

  GameLogic game1;
};

TEST_F(GameLogicTest, TestingIntialPositionOfPieces) {
  std::vector<std::vector<Piece> > expBoard = constructInitialBoard();

  ASSERT_EQ(expBoard, game1.getWorldModel().board);
}

// TEST_F(GameLogicTest, TestingTwoCommandsInTheSameTableEntry) {
//   Command command1; command1.coordinate.x = 1; command1.coordinate.y = 1;
//   Command command2; command2.coordinate.x = 0; command2.coordinate.y = 2;
//   game1.update(command1, 9090);
//   game1.update(command2, 9091);
//   ASSERT_FALSE(game1.update(command1, 9090));
//   ASSERT_FALSE(game1.update(command2, 9091));

//   std::vector<std::vector<Marker> > expTable;
//   for (int i = 0; i < 9; i++) {
//     std::vector<Marker> line;    
//     for (int i = 0; i < 9; i++) {
//       line.push_back(Marker::UNMARKED);
//     }
//     expTable.push_back(line);
//   }
//   expTable[1][1] = Marker::X;
//   expTable[0][2] = Marker::O;

//   ASSERT_EQ(expTable, game1.getWorldModel().table);

//   ASSERT_FALSE(game1.update(command1, 9091));
//   ASSERT_FALSE(game1.update(command2, 9090));
//   ASSERT_EQ(expTable, game1.getWorldModel().table);
// }

// TEST_F(GameLogicTest, TestingInsertingPieceIntoATrappedSpot) {

//   std::vector<std::vector<int> > indexes {
//     {0,1,9090},
//     {8,8,9091},
//     {1,0,9090},
//     {7,7,9091},
//     {1,2,9090},
//     {6,6,9091},
//     {2,1,9090}
//   };
//   Command command;
//   for(const auto& line : indexes) {
//     command.coordinate.x = line[0];
//     command.coordinate.y = line[1];
//     game1.update(command, line[2]);
//   }
//   command.coordinate.x = 1;
//   command.coordinate.y = 1;
//   EXPECT_FALSE(game1.update(command, 9091));

//   std::vector<std::vector<Marker> > expTable;
//   for (int i = 0; i < 9; i++) {
//     std::vector<Marker> line;    
//     for (int i = 0; i < 9; i++) {
//       line.push_back(Marker::UNMARKED);
//     }
//     expTable.push_back(line);
//   }
//   expTable[0][1] = Marker::X;
//   expTable[8][8] = Marker::O;
//   expTable[1][0] = Marker::X;
//   expTable[7][7] = Marker::O;
//   expTable[1][2] = Marker::X;
//   expTable[6][6] = Marker::O;
//   expTable[2][1] = Marker::X;

//   ASSERT_EQ(expTable, game1.getWorldModel().table);
// }

// TEST_F(GameLogicTest, TestingOneTrappedPieceRemoval) {

//   std::vector<std::vector<int> > indexes {
//     {0,1,9090},
//     {1,1,9091},
//     {1,0,9090},
//     {8,8,9091},
//     {1,2,9090},
//     {7,7,9091},
//     {2,1,9090}
//   };
//   Command command;
//   for(const auto& line : indexes) {
//     command.coordinate.x = line[0];
//     command.coordinate.y = line[1];
//     game1.update(command, line[2]);
//   }

//   std::vector<std::vector<Marker> > expTable;
//   for (int i = 0; i < 9; i++) {
//     std::vector<Marker> line;    
//     for (int i = 0; i < 9; i++) {
//       line.push_back(Marker::UNMARKED);
//     }
//     expTable.push_back(line);
//   }
//   expTable[0][1] = Marker::X;
//   expTable[1][1] = Marker::UNMARKED;
//   expTable[1][0] = Marker::X;
//   expTable[8][8] = Marker::O;
//   expTable[1][2] = Marker::X;
//   expTable[7][7] = Marker::O;
//   expTable[2][1] = Marker::X;

//   ASSERT_EQ(expTable, game1.getWorldModel().table);
// }

// TEST_F(GameLogicTest, TestingTwoTrappedPieceRemoval) {

//   std::vector<std::vector<int> > indexes {
//     {0,1,9090},
//     {1,1,9091},
//     {1,0,9090},
//     {1,2,9091},
//     {2,1,9090},
//     {8,8,9091},
//     {0,2,9090},
//     {7,7,9091},
//     {1,3,9090},
//     {6,6,9091},
//     {2,2,9090},

//   };
//   Command command;
//   for(const auto& line : indexes) {
//     command.coordinate.x = line[0];
//     command.coordinate.y = line[1];
//     game1.update(command, line[2]);
//   }

//   std::vector<std::vector<Marker> > expTable;
//   for (int i = 0; i < 9; i++) {
//     std::vector<Marker> line;    
//     for (int i = 0; i < 9; i++) {
//       line.push_back(Marker::UNMARKED);
//     }
//     expTable.push_back(line);
//   }
//   expTable[0][1] = Marker::X;
//   expTable[1][1] = Marker::UNMARKED;
//   expTable[1][0] = Marker::X;
//   expTable[1][2] = Marker::UNMARKED;
//   expTable[2][1] = Marker::X;
//   expTable[8][8] = Marker::O;
//   expTable[0][2] = Marker::X;
//   expTable[7][7] = Marker::O;
//   expTable[1][3] = Marker::X;
//   expTable[6][6] = Marker::O;
//   expTable[2][2] = Marker::X;

//   ASSERT_EQ(expTable, game1.getWorldModel().table);
// }

// TEST_F(GameLogicTest, TestingDrawAndFinish) {

//   std::vector<std::vector<int> > indexes {
//     {0,1,9090},
//     {1,1,9091},
//     {-1,-1,9090},
//     {-1,-1,9091}
//   };
//   Command command;
//   for(const auto& line : indexes) {
//     command.coordinate.x = line[0];
//     command.coordinate.y = line[1];
//     game1.update(command, line[2]);
//   }

//   EXPECT_TRUE(game1.isFinished());
//   EXPECT_EQ("-1", game1.getWinner());
// }

// TEST_F(GameLogicTest, TestingWinAndFinish) {

//   std::vector<std::vector<int> > indexes {
//     {0,1,9090},
//     {8,8,9091},
//     {1,0,9090},
//     {7,7,9091},
//     {-1,-1,9090},
//     {-1,-1,9091}
//   };
//   Command command;
//   for(const auto& line : indexes) {
//     command.coordinate.x = line[0];
//     command.coordinate.y = line[1];
//     game1.update(command, line[2]);
//   }

//   EXPECT_TRUE(game1.isFinished());
//   EXPECT_EQ("9090", game1.getWinner());
// }

// TEST_F(GameLogicTest, TestingCaptureWinAndFinish) {

//   std::vector<std::vector<int> > indexes {
//     {0,1,9090},
//     {0,0,9091},
//     {1,0,9090},
//     {0,6,9091},
//     {8,8,9090},
//     {1,7,9091},
//     {-1,-1,9090},
//     {-1,-1,9091}
//   };
//   Command command;
//   for(const auto& line : indexes) {
//     command.coordinate.x = line[0];
//     command.coordinate.y = line[1];
//     game1.update(command, line[2]);
//   }

//   EXPECT_TRUE(game1.isFinished());
//   EXPECT_EQ("9090", game1.getWinner());
// }

// TEST_F(GameLogicTest, TestingBoardFullAndFinish) {

//   std::vector<std::vector<int> > indexes;
//   for (int i = 0; i < 4; i++) {
//     for (int j = 0; j < 9; j++) {
//       std::vector<int> play;
//       play.push_back(i);
//       play.push_back(j);
//       play.push_back(9090);
//       indexes.push_back(play);
//       play.clear();
//       play.push_back(8-i);
//       play.push_back(j);
//       play.push_back(9091);
//       indexes.push_back(play);
//     }
//   }
//   for (int i = 0; i < 4; i++) {
//     std::vector<int> play;
//       play.push_back(4);
//       play.push_back(i);
//       play.push_back(9090);
//       indexes.push_back(play);
//       play.clear();
//       play.push_back(4);
//       play.push_back(8-i);
//       play.push_back(9091);
//       indexes.push_back(play);
//   }
//   indexes.push_back({4, 4, 9090});

//   Command command;
//   for(const auto& line : indexes) {
//     command.coordinate.x = line[0];
//     command.coordinate.y = line[1];
//     game1.update(command, line[2]);
//   }

//   std::vector<std::vector<Marker> > expTable;
//   Marker marker = Marker::X;
//   for (int i = 0; i < 9; i++) {
//     std::vector<Marker> line;    
//     for (int j = 0; j < 9; j++) {
//       if (i >= 4 && j >= 5) {
//         marker = Marker::O;
//       }
//       line.push_back(marker);
//     }
//     expTable.push_back(line);
//   }

//   EXPECT_EQ(expTable, game1.getWorldModel().table);
//   EXPECT_TRUE(game1.isFinished());
//   EXPECT_EQ("9090", game1.getWinner());
// }

}}  // namespaces

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}