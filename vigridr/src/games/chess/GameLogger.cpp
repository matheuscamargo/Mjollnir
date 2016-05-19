#include "GameLogger.h"

#include <fstream>
#include <vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

namespace mjollnir { namespace vigridr {

std::vector<WorldModel> wmList;
GameDescription gd1,gd2;
std::string player1Id,player2Id;

ptree createPt(const WorldModel& wm) {
  ptree wmPt;
  ptree tablePt;
  for (auto line : wm.board) {
    ptree linePt;
    for (auto elem : line) {
      std::string elemStr =
        (elem == Piece::PAWN) ? "P" : 
        (elem == Piece::TOWER) ? "T" :
        (elem == Piece::HORSE) ? "H" :   
        (elem == Piece::BISHOP) ? "B" :  
        (elem == Piece::QUEEN) ? "Q" :       
        (elem == Piece::KING) ? "K" : "-";
      linePt.push_back(std::make_pair("", ptree(elemStr)));
    }
    tablePt.push_back(std::make_pair("", linePt));
  }
  wmPt.push_back(std::make_pair("board",tablePt));
  return wmPt;
}

ptree createGameDescriptionPt() {
  ptree gdPt;
  gdPt.put(player1Id, gd1.myColor == PlayerColor::BLACK ? "Black":"White");
  gdPt.put(player2Id, gd2.myColor == PlayerColor::BLACK ? "Black":"White");
  return gdPt;
}

void GameLogger::logWorldModel(const WorldModel& wm, const TotalWorldModel& twm) {
  wmList.push_back(wm);
}

char toChar(Piece p) {
  switch(p) {
    case Piece::PAWN: return 'P';
    case Piece::TOWER: return 'T';
    case Piece::HORSE: return 'H';
    case Piece::BISHOP: return 'B';
    case Piece::QUEEN: return 'Q';
    case Piece::KING: return 'K';
    default: return '-';
  }
}

void GameLogger::printWorldModel(const WorldModel& wm, const TotalWorldModel& twm) {
  std::ostringstream oss;
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      oss << toChar(wm.board[i][j]) << ' ';
    }
    oss << std::endl;
  }
  oss << std::endl;
  std::cerr << oss.str();
}

void GameLogger::logGameDescription(const GameDescription& description1,
                                    const std::string& player1,
                                    const GameDescription& description2,
                                    const std::string& player2) {
  gd1 = description1;
  gd2 = description2;
  player1Id = player1;
  player2Id = player2;
}


void GameLogger::flushLog() {
  ptree wmListPt;
  for (auto wm : wmList) {
    wmListPt.push_back(std::make_pair("", createPt(wm)));
  }
  ptree gamePt;
  gamePt.push_back(std::make_pair("wmList", wmListPt));
  gamePt.push_back(std::make_pair("gameDescription",
                                  createGameDescriptionPt()));
  std::ofstream file;
  file.open("logs");
  write_json (file, gamePt, false);
  file.close();
}

}}
