#include <chrono>
#include <exception>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <thread>
#include <unistd.h>

#include <gflags/gflags.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "../thrifts/gen-cpp/Game.h"
#include "ClientLogic.h"

DEFINE_int32(port, 9090, "Port used to connect with the server.");

const char* const kVersion = "v1.1";
const char* const kUsageMessage =
  "This program is the client for mjollnir matches";

using ::apache::thrift::protocol::TBinaryProtocol;
using ::apache::thrift::protocol::TProtocol;
using ::apache::thrift::TException;
using ::apache::thrift::transport::TBufferedTransport;
using ::apache::thrift::transport::TSocket;
using ::apache::thrift::transport::TTransport;

using ::mjollnir::vigridr::Command;
using ::mjollnir::vigridr::GameClient;
using ::mjollnir::vigridr::GameInfo;
using ::mjollnir::vigridr::GameInit;
using ::mjollnir::vigridr::GameStatus;
using ::mjollnir::vigridr::WorldModel;

void playGame(GameClient& client) {
  GameInit gameInit;
  client.ready(gameInit);
  init(gameInit);
  GameInfo gameInfo = gameInit.gameInfo;
  while (true) {
    client.getGameInfo(gameInfo);
    if (gameInfo.gameStatus == GameStatus::FINISHED) {
      endOfGame(gameInfo.gameResult);
      break;
    }
    if (gameInfo.isMyTurn) {
      Command command = playTurn(gameInfo.worldModel, gameInfo.cycle);
      client.sendCommand(command);
    }
  }
}

int main(int argc, char** argv) {
  gflags::SetVersionString(kVersion);
  gflags::SetUsageMessage(kUsageMessage);
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  try {
    boost::shared_ptr<TTransport> socket(new TSocket("localhost", FLAGS_port));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    GameClient client(protocol);
    transport->open();
    playGame(client);
    transport->close();
  } catch (TException &tx) {
    printf("ERROR: %s\n", tx.what());
  }
}
