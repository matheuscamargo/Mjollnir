from WorldModel.ttypes import PlayerColor
from Command.ttypes import Command
from Command.ttypes import Coordinate
from GameModel.ttypes import GameStatus
from GameResult.ttypes import GameResult

from random import randint
from random import choice

class Solution:
    def __init__(self, gameInit):
        """
        Constructor: called at the beginning of the game.
        You may do initialization here.

        Parameter:
            gameInit - contains a field named gameDescription, which itself contains a field named myType.
                       myType is of type int, and can have one of the Marker values: UNMARKED, X and O.
        """
        print "Python Client"
        print "PlayerColor: " + PlayerColor._VALUES_TO_NAMES[gameInit.gameDescription.myColor]

    def play_turn(self, wm, moveList, turn):
        """
        This method is called once for every turn.
        This specific example solution returns a random valid position.

        Parameters:
            wm   - an instance of the WorldModel class that contains a field called table which is a list of lists of ints,
                   which can have on of the Marker values.
            turn - an integer, the index of the turn.
                   If you receive twice the same number, don't worry, just ignore it.

        Returns:
            A Command instance - a Command contains a field called coordinate of class coordinate.
                                 A Coordinate contains two fields of type int, x and y.
        """
        command = Command()

        if moveList:
            command = choice(moveList)
            print "%d: %s to %s" % (turn, repr(command.coordFrom), repr(command.coordTo))

        return command

    def end_of_game(self, result):
        """
        This method is called at the end of the game.

        Parameters:
            result - an integer, which can be GameResult.WON, GameResult.TIED or GameResult.LOST.
        """
        print "End of game - " + GameResult._VALUES_TO_NAMES[result]
