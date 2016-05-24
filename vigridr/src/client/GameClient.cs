using System;
using System.Threading;
using Thrift;
using Thrift.Protocol;
using Thrift.Server;
using Thrift.Transport;
using System.Diagnostics;

public class CSharpClient
{
    public static void playGame(Game.Client client) {
        GameInit gameInit = client.ready();
        Solution solution = new Solution(gameInit);

        GameInfo gameInfo;
        while (true) {
            gameInfo = client.getGameInfo();
            if (gameInfo.GameStatus == GameStatus.FINISHED)
            {
                solution.EndOfGame(gameInfo.GameResult);
                break;
            }
            if (gameInfo.IsMyTurn)
            {
                Command command = solution.playTurn(gameInfo.WorldModel, gameInfo.Cycle);
                client.sendCommand(command);
            }
        }
    }

    public static void Main(string[] args)
    {
        int port = 9090;
        for (int i = 0; i < args.Length; i++)
        {
            switch(args[i])
            {
                case "--port":
                    port = int.Parse(args[++i]);
                    break;
                case "--help":
                    Console.WriteLine("--port: Port used to connect with the server. (int)");
                    break;
            }
        }

        try
        {
            TSocket tSocket = new TSocket("localhost", port);
            tSocket.TcpClient.NoDelay = true;
            TTransport transport = tSocket;
            TProtocol protocol = new TBinaryProtocol(transport);
            Game.Client client = new Game.Client(protocol);
            transport.Open();
            playGame(client);
            transport.Close();
        }
        catch (TApplicationException x)
        {
            Console.WriteLine(x.StackTrace);
        }
    }
}
