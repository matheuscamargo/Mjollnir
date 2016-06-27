import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

public class ClientLogic {
    public Random random;
    private int me;
    private int other;
    private int lastTurn;
    private Command lastCommand;
    private int direction;
    private int start;
    private int end;

    /*
     * Constructor: called at the beginning of the game.
     * You may do initialization here.
     *
     * Parameter:
     *     gameInit - contains an attribute named GameDescription, which itself contains an attribute named myColor.
     *                MyColor is of type PlayerColor, which is an enum. PlayerColor has two fields: RED and WHITE.
     */
    public ClientLogic(GameInit gameInit) {
        random = new Random();
        System.out.println("Java Backgammon Client");

        PlayerColor color = gameInit.gameDescription.myColor;
        System.out.println("PlayerColor: " + color);

        this.me = color.getValue();
        this.lastTurn = -1;
        this.lastCommand = null;

        if (color == PlayerColor.RED) {
            this.other = PlayerColor.WHITE.getValue();
            this.direction = +1;
            this.start = 0;
            this.end = 23;
        }
        else {
            this.other = PlayerColor.RED.getValue();
            this.direction = -1;
            this.start = 23;
            this.end = 0;
        }
    }

    /*
     * This method is called once for every turn.
     * This specific example solution tries to move the checkers at the highest points.
     * However, it is not complete, so it sometimes sends invalid commands.
     *
     * Parameters:
     *     wm   - an instance of the WorldModel class that contains the following attributes:
     *            bar       - type Point. The number of checkers for each player in the bar.
     *            board     - List of Point. Always contains 24 elements.
     *            borne_off - type Point. The number of checkers that each player has borne off.
     *            dice      - List of Integer. Always contains 2 elements.
     *            A Point is an alias for a List of Integer, with 2 elements, that represent
     *            the number of red and white checkers in that point, in that order. Hint: rembember that RED=0 and WHITE=1.
     *            Remember that a "point" is that triangle on the board where the checkers can be.
     *     turn - the index of the turn.
     *            If you receive twice the same number, don't worry, just ignore it.
     *
     * Returns:
     *     A Command instance - a Command contains an attribute called moves, which is a List of Move.
     *                          A Move contains two attributes of type int, src and dst.
     *                          src and dst must be in the interval [0, 24).
     *                          Additionally, src can be CommandConstants.FROM_BAR and dst can be CommandConstants.BEAR_OFF.
     */
    public Command playTurn(WorldModel wm, List<Command> moveList, int turn) {

        System.out.print(turn + ": " + Extensions.Representation(wm.dice) + " ");

        int size = moveList.size();
        Command command = new Command();
        command.moves = new ArrayList<Move>();

        if (size > 0) {
            int index = random.nextInt(size);
            command = moveList.get(index); 
        }  
        // Finally send command
        System.out.println("Command: " + Extensions.Representation(command));
        this.lastCommand = command;
        return command;
    }

    /*
     * This method is called at the end of the game.
     *
     * Parameters:
     *     result - an instance of the GameResult class, which contains two boolean attributes, Won and Invalid.
     *              The Invalid property is true if you lost because you sent an invalid command.
     */
    public void endOfGame(GameResult result) {
        System.out.println("End of game - " + (result.won ? "Won!" : "Lost..."));
        if (result.invalid) {
            System.out.println("[WARNING] Invalid command");
        }
    }

    /*
     * Helper function to convert a List of List of Integer to an array of arrays of int
     * (aka matrix of int).
     */
    private static int[][] toMatrix(List<List<Integer>> board) {
        int[][] matrix = new int[board.size()][];

        int i = 0;
        for (List<Integer> point : board) {
            matrix[i] = new int[] { point.get(0), point.get(1) };
            i++;
        }

        return matrix;
    }
}

/*
 * Helper class to pretty print some values. They are not mandatory for a solution.
 */
class Extensions {
    public static String Representation(Move move) {
        return move.src + "->" + move.dst;
    }

    public static String Representation(List<Integer> l) {
        StringBuilder builder = new StringBuilder("[ ");
        for (int i : l) {
            builder.append(i + ", ");
        }
        builder.append("]");
        return builder.toString();
    }

    public static String Representation(Command command) {
        StringBuilder builder = new StringBuilder("[ ");
        for (Move move : command.moves) {
            builder.append(Extensions.Representation(move) + ", ");
        }
        builder.append("]");
        return builder.toString();
    }
}
