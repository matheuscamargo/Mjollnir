import sys
import glob
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--port", \
                    help="Port used to connect with the server.", \
                    type=int)
parser.add_argument("--thrift", \
                    help="Path to lib folder in which thrift is.", \
                    type=str)
args = parser.parse_args()
if not args.port:
    args.port = 9090
if not args.thrift:
    args.thrift = '../../third-parties/python/lib'

sys.path.append('gen-py')
sys.path.insert(0, glob.glob(args.thrift)[0])

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

from ClientLogic import Solution
from GameServer import Game
from Command.ttypes import Command
from GameModel.ttypes import GameStatus

def play_game(client):
    gameInit = client.ready()
    solution = Solution(gameInit)
    
    while True:
        gameInfo = client.getGameInfo()
        if gameInfo.gameStatus == GameStatus.FINISHED:
            solution.end_of_game(gameInfo.gameResult);
            break
        if gameInfo.isMyTurn:
            command = solution.play_turn(gameInfo.worldModel, gameInfo.moveList, gameInfo.cycle)
            client.sendCommand(command)
        

def run_client():
    if not args.port:
        args.port = 9090

    try:
        transport = TSocket.TSocket('localhost', args.port)
        transport = TTransport.TBufferedTransport(transport)
        protocol = TBinaryProtocol.TBinaryProtocol(transport)
        client = Game.Client(protocol)
        transport.open()
        play_game(client)
        transport.close()
    except Thrift.TException, tx:
        print 'ERROR: %s' % (tx.message)


if __name__ == "__main__":
    run_client()
