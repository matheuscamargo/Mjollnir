
import pdb

from game import Game
from uuid import uuid4

class Logger():
    def error(self, msg):
        print msg
    def info(self, msg):
        print msg

with Game('ttt1.cs', 'ttt2.py', 'croata', 'roim', 'ttt', 'tictactoe', uuid4(), Logger()) as game:
    #pdb.set_trace()
    game.download()
    game.compile()
    game.run()
    game.upload()
    for key, value in game.result.items():
        print key, '=>', value
