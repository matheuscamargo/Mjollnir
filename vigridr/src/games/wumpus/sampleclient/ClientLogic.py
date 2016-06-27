from Command.ttypes import Action
from Command.ttypes import Command
from GameModel.ttypes import GameStatus

from random import randint
import random

class Solution:
    def __init__(self, gameInit):
        """
        Constructor: called at the beginning of the game.
        You may do initialization here.

        Parameter:
            gameInit - not used for Wumpus
        """
        print "Python Client"

    def play_turn(self, wm, moveList, turn):
        """
        This method is called once for every turn.
        This specific example solution returns a random action.

        Parameters:
            wm -   an instance of the WorldModel class that contains an attribute called sensors of class Sensors.
                   Sensors contains the boolean attributes: breeze, stench, glitter, bump and scream.
            turn - an integer, the index of the turn.
                   If you receive twice the same number, don't worry, just ignore it.
        Returns:
            A Command instance - a Command contains an attribute called action of type int.
                                 action must be one of the Action attributes: FORWARD, TURNRIGHT, TURNLEFT, STAY, SHOOT, GRAB and CLIMB.
        """
        command = Command()

        if moveList:
            command = random.choice(moveList)
            print str(turn) + ": " + str(command.action)

        return command

    def end_of_game(self, result):
        """
        This method is called at the end of the game.

        Parameters:
            result - an instance of the GameResult class, that has only one int field, score.
        """
        print "End of game - Score: " + str(result.score)
