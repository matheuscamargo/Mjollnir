import java.util.List;
import java.util.Random;

public class ClientLogic {
    public Random random;

    /*
     * Constructor: called at the beginning of the game.
     * You may do initialization here.
     *
     * Parameter:
     *     gameInit - not used for Wumpus
     */
    public ClientLogic(GameInit gameInit) {
        random = new Random();

        System.out.println("Java Client");
    }

    /*
     * This method is called once for every turn.
     * This specific example solution returns a random action.
     *
     * Parameters:
     *     wm   - an instance of the WorldModel class that contains a field called sensors of class Sensors.
     *            Sensors contains the boolean fields: breeze, stench, glitter, bump and scream.
     *     turn - the index of the turn.
     *            If you receive twice the same number, don't worry, just ignore it.
     *
     * Returns:
     *     A Command instance - a Command contains a field called action of enum Action.
     *                          Action fields: FORWARD, TURNRIGHT, TURNLEFT, STAY, SHOOT, GRAB and CLIMB.
     */
    public Command playTurn(WorldModel wm, List<Command> moveList, int turn) {
        int index = random.nextInt(moveList.size());
        Command command = moveList.get(index);  

        System.out.println("Command: " + command.action);

        return command;
    }

    /*
     * This method is called at the end of the game.
     *
     * Parameters:
     *     result - an instance of the GameResult class, that has only one int attribute, score.
     */
    public void endOfGame(GameResult result) {
        System.out.println("End of game - Score: " + result.score);
    }
}

