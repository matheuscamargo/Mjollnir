from Command.ttypes import Command, Move
from Command.constants import FROM_BAR, BEAR_OFF
from GameDescription.ttypes import PlayerColor
from GameModel.ttypes import GameStatus
from GameResult.ttypes import GameResult

RED, WHITE, _VALUES_TO_NAMES = PlayerColor.RED, PlayerColor.WHITE, PlayerColor._VALUES_TO_NAMES

from random import choice

class Solution:
    def __init__(self, gameInit):
        """
        Constructor: called at the beginning of the game.
        You may do initialization here.

        Parameter:
            gameInit - contains a field named gameDescription, which itself contains a field named myColor.
                       myColor is an int, which can be RED or WHITE.
        """
        print "Python Backgammon Client"
        print "PlayerColor: " + _VALUES_TO_NAMES[gameInit.gameDescription.myColor]
        self.color = gameInit.gameDescription.myColor
        self.last_turn = -1
        self.last_command = None

        if self.color == RED:
            self.other = WHITE
            self.direction = +1
            self.start = 0
            self.end = 23
        else:
            self.other = RED
            self.direction = -1
            self.start = 23
            self.end = 0

    def play_turn(self, wm, moveList, turn):
        """
        This method is called once for every turn.
        This specific example solution tries to move the checkers at the highest points.
        However, it is not complete, so it sometimes sends invalid commands.

        Parameters:
            wm   - an instance of the WorldModel class that contains the following fields:
                   bar       - type Point. The number of checkers for each player in the bar.
                   board     - list of Point. Always contains 24 elements.
                   borne_off - type Point. The number of checkers that each player has borne off.
                   dice      - list of int. Always contains 2 elements.
                   A Point is an alias for a list of int, with 2 elements, that represent
                   the number of red and white checkers in that point, in that order. Hint: rembember that RED=0 and WHITE=1.
                   Remember that a "point" is that triangle on the board where the checkers can be.
            turn - the index of the turn.
                   If you receive twice the same number, don't worry, just ignore it.

        Returns:
            A Command instance - a Command contains a field called moves, which is a list of Move.
                                 A Move contains two fields of type int, src and dst.
                                 src and dst must be in the interval [0, 24).
                                 Additionally, src can be FROM_BAR and dst can be BEAR_OFF.
        """

        command = Command()

        if moveList:
            command = choice(moveList)
            print str(turn) + ": " + str(command.moves)

        return command

    def end_of_game(self, result):
        """
         * This method is called at the end of the game.
         *
         * Parameters:
         *     result - an instance of the GameResult class, which contains two booleans fields: won and invalid.
         *              The invalid field is True if you lost because you sent an invalid command.
        """
        print "End of game - " + ("Won!" if result.won else "Lost...")
        if result.invalid:
            print "[WARNING] Invalid command"

