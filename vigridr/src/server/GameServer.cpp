#include <memory>
#include <functional>
#include <thread>

#include <gflags/gflags.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "GameManager.h"
#include "GameService.h"
#include "../utils/Log.h"
#include "../utils/StringUtils.h"
#include "../thrifts/gen-cpp/Game.h"

DEFINE_int32(port1, 9090, "Port used by the first client.");
DEFINE_int32(port2, 9091, "Port used by the second client.");

const char* const kVersion = "v1.1";
const char* const kUsageMessage =
  "This program is the server for mjollnir matches";

using ::apache::thrift::protocol::TBinaryProtocolFactory;
using ::apache::thrift::protocol::TProtocolFactory;
using ::apache::thrift::server::TProcessor;
using ::apache::thrift::server::TThreadedServer;
using ::apache::thrift::transport::TBufferedTransportFactory;
using ::apache::thrift::transport::TServerSocket;
using ::apache::thrift::transport::TServerTransport;
using ::apache::thrift::transport::TTransportFactory;

using mjollnir::vigridr::GameManager;
using mjollnir::vigridr::GameProcessor;
using mjollnir::vigridr::GameService;

int main(int argc, char **argv) {
  gflags::SetVersionString(kVersion);
  gflags::SetUsageMessage(kUsageMessage);
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::vector<int32_t> portsInts = {FLAGS_port1, FLAGS_port2};

  auto gameManager = std::make_shared<GameManager>(portsInts);
  auto serviceInit = [&](int32_t port) {
    boost::shared_ptr<GameService> handler(new GameService(gameManager, port));

    boost::shared_ptr<TProcessor> processor(new GameProcessor(handler));
    boost::shared_ptr<TServerTransport> serverTransport(
      new TServerSocket(port));
    boost::shared_ptr<TTransportFactory> transportFactory(
      new TBufferedTransportFactory());
    boost::shared_ptr<TProtocolFactory> protocolFactory(
      new TBinaryProtocolFactory());

    // Creates one thread for each connection, so blocking one connection
    // doesnt block server
    TThreadedServer server(processor, serverTransport,
                         transportFactory, protocolFactory);

    // finishing server when game ends
    gameManager->onGameEnd([&] () {
      LOG("Stopping server");
      server.stop();
    });
    LOG("Starting server...");
    server.serve();
    LOG("Done");
  };

  std::thread player1Service(serviceInit, FLAGS_port1);
  std::thread player2Service(serviceInit, FLAGS_port2);

  player1Service.join();
  player2Service.join();
  
  //Workaround solution so that clients have time to finish by themselves
  //Before the server is killed
  std::chrono::seconds sleepTime(1);
  std::this_thread::sleep_for(sleepTime);
  return 0;
}

