#include "../server/GameLogic.h"
#include "gtest/gtest.h"


namespace mjollnir { namespace vigridr {

class GameLogicTest : public ::testing::Test {
 protected:

  GameLogicTest() : game1(9090, 9091) { }

  void CleanTable() {
    Coordinate coord;
    for(size_t i = 0; i < 3; ++i) {
      for(size_t j = 0; j < 3; ++j) {
        coord.x = i; coord.y = j;
        game1.setTableCoordinate(coord, Marker::UNMARKED);
      }
    }
    game1.setHasFinished(false);
    game1.setWinner("-1");
  }

  GameLogic game1;
};

TEST_F(GameLogicTest, TestingPlayersMarkersTypes) {
  Command command1; command1.coordinate.x = 1; command1.coordinate.y = 1;
  Command command2; command2.coordinate.x = 0; command2.coordinate.y = 2;
  ASSERT_TRUE(game1.update(command1, 9090));
  ASSERT_TRUE(game1.update(command2, 9091));

  std::vector<std::vector<Marker> > expTable;
  for (int i = 0; i < 9; i++) {
    std::vector<Marker> line;    
    for (int i = 0; i < 9; i++) {
      line.push_back(Marker::UNMARKED);
    }
    expTable.push_back(line);
  }
  expTable[1][1] = Marker::X;
  expTable[0][2] = Marker::O;

  ASSERT_EQ(expTable, game1.getWorldModel().table);
}

TEST_F(GameLogicTest, TestingTwoCommandsInTheSameTableEntry) {
  Command command1; command1.coordinate.x = 1; command1.coordinate.y = 1;
  Command command2; command2.coordinate.x = 0; command2.coordinate.y = 2;
  game1.update(command1, 9090);
  game1.update(command2, 9091);
  ASSERT_FALSE(game1.update(command1, 9090));
  ASSERT_FALSE(game1.update(command2, 9091));

  std::vector<std::vector<Marker> > expTable;
  for (int i = 0; i < 9; i++) {
    std::vector<Marker> line;    
    for (int i = 0; i < 9; i++) {
      line.push_back(Marker::UNMARKED);
    }
    expTable.push_back(line);
  }
  expTable[1][1] = Marker::X;
  expTable[0][2] = Marker::O;

  ASSERT_EQ(expTable, game1.getWorldModel().table);

  ASSERT_FALSE(game1.update(command1, 9091));
  ASSERT_FALSE(game1.update(command2, 9090));
  ASSERT_EQ(expTable, game1.getWorldModel().table);
}

TEST_F(GameLogicTest, TestingInsertingPieceIntoATrappedSpot) {

  std::vector<std::vector<int> > indexes {
    {0,1,9090},
    {8,8,9091},
    {1,0,9090},
    {7,7,9091},
    {1,2,9090},
    {6,6,9091},
    {2,1,9090}
  };
  Command command;
  for(const auto& line : indexes) {
    command.coordinate.x = line[0];
    command.coordinate.y = line[1];
    game1.update(command, line[2]);
  }
  command.coordinate.x = 1;
  command.coordinate.y = 1;
  EXPECT_FALSE(game1.update(command, 9091));

  std::vector<std::vector<Marker> > expTable;
  for (int i = 0; i < 9; i++) {
    std::vector<Marker> line;    
    for (int i = 0; i < 9; i++) {
      line.push_back(Marker::UNMARKED);
    }
    expTable.push_back(line);
  }
  expTable[0][1] = Marker::X;
  expTable[8][8] = Marker::O;
  expTable[1][0] = Marker::X;
  expTable[7][7] = Marker::O;
  expTable[1][2] = Marker::X;
  expTable[6][6] = Marker::O;
  expTable[2][1] = Marker::X;

  ASSERT_EQ(expTable, game1.getWorldModel().table);
}

TEST_F(GameLogicTest, TestingOneTrappedPieceRemoval) {

  std::vector<std::vector<int> > indexes {
    {0,1,9090},
    {1,1,9091},
    {1,0,9090},
    {8,8,9091},
    {1,2,9090},
    {7,7,9091},
    {2,1,9090}
  };
  Command command;
  for(const auto& line : indexes) {
    command.coordinate.x = line[0];
    command.coordinate.y = line[1];
    game1.update(command, line[2]);
  }

  std::vector<std::vector<Marker> > expTable;
  for (int i = 0; i < 9; i++) {
    std::vector<Marker> line;    
    for (int i = 0; i < 9; i++) {
      line.push_back(Marker::UNMARKED);
    }
    expTable.push_back(line);
  }
  expTable[0][1] = Marker::X;
  expTable[1][1] = Marker::UNMARKED;
  expTable[1][0] = Marker::X;
  expTable[8][8] = Marker::O;
  expTable[1][2] = Marker::X;
  expTable[7][7] = Marker::O;
  expTable[2][1] = Marker::X;

  ASSERT_EQ(expTable, game1.getWorldModel().table);
}

TEST_F(GameLogicTest, TestingTwoTrappedPieceRemoval) {

  std::vector<std::vector<int> > indexes {
    {0,1,9090},
    {1,1,9091},
    {1,0,9090},
    {8,8,9091},
    {1,2,9090},
    {7,7,9091},
    {2,1,9090}
  };
  Command command;
  for(const auto& line : indexes) {
    command.coordinate.x = line[0];
    command.coordinate.y = line[1];
    game1.update(command, line[2]);
  }

  std::vector<std::vector<Marker> > expTable;
  for (int i = 0; i < 9; i++) {
    std::vector<Marker> line;    
    for (int i = 0; i < 9; i++) {
      line.push_back(Marker::UNMARKED);
    }
    expTable.push_back(line);
  }
  expTable[0][1] = Marker::X;
  expTable[1][1] = Marker::UNMARKED;
  expTable[1][0] = Marker::X;
  expTable[8][8] = Marker::O;
  expTable[1][2] = Marker::X;
  expTable[7][7] = Marker::O;
  expTable[2][1] = Marker::X;

  ASSERT_EQ(expTable, game1.getWorldModel().table);
}

}}  // namespaces

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}