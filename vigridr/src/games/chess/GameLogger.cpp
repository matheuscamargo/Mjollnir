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
      std::string elemStr;
      if (elem.owner == PlayerColor::BLACK) {
        elemStr =
          (elem.type == Type::PAWN) ? "P" : 
          (elem.type == Type::TOWER) ? "T" :
          (elem.type == Type::HORSE) ? "H" :   
          (elem.type == Type::BISHOP) ? "B" :  
          (elem.type == Type::QUEEN) ? "Q" :       
          (elem.type == Type::KING) ? "K" : "-";
      }
      else {        
        elemStr =
          (elem.type == Type::PAWN) ? "p" : 
          (elem.type == Type::TOWER) ? "t" :
          (elem.type == Type::HORSE) ? "h" :   
          (elem.type == Type::BISHOP) ? "b" :  
          (elem.type == Type::QUEEN) ? "q" :       
          (elem.type == Type::KING) ? "k" : "-";
      }
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

void GameLogger::printWorldModel(const WorldModel& wm, const TotalWorldModel& twm) {
  std::ostringstream oss;

  oss << "    ╔════════════════╗" << std::endl;

  for (int i=0; i<8; i++) {

    if( i == 3 )
      oss << 'x';
    else
      oss << ' ';
    oss << ' ' << i << " ║";

    for (int j=0; j<8; j++) {
      oss << toChar(wm.board[i][j]);
      if( wm.board[i][j].moved ){
        oss << '*';
      }
      else {
        oss << ' ';
      }
    }
    oss << "║" << std::endl;
  }

  oss << "    ╚════════════════╝" << std::endl;
  oss << "     0 1 2 3 4 5 6 7 " << std::endl;
  oss << "            y        " << std::endl;

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
