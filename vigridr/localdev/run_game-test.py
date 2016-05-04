from time import sleep
from os import path
from subprocess import Popen, STDOUT
from threading import Timer

try:    
    # Construction of server parameters
    server_kwargs = {
        'args': ['./server'],
        'cwd': 'server',
        'stdout': open(path.join('server', 'result'), 'w'),
        'stderr': open(path.join('server', 'output'), 'w'),
    }
    
    # Player parameters (id and port)        
    for idx in [0, 1]:
        server_kwargs['args'].append('--player' + str(idx + 1))
        server_kwargs['args'].append('uid' + str(idx + 1)) # I guess uid can be any random thing (but if is COMPUTER there's something special)
    for idx in [0, 1]:
        server_kwargs['args'].append('--port' + str(idx + 1))
        server_kwargs['args'].append('909' + str(idx))
        
    # Construction of client parameters
    client_kwargs = []
    for idx in [0,1]:
        client_kwargs.append({
            'args': ['./client', '--port', '909' + str(idx)],
            'cwd': 'client' + str(idx + 1),
            'stdout': open(path.join('client' + str(idx + 1), 'output'), 'w'),
            'stderr': STDOUT,
        })
    
    # Executing server
    print 'starting server'
    server_process = Popen(**server_kwargs)
    sleep(0.5)

    # Executing clients
    client_processes = []
    for idx, client_kwarg in enumerate(client_kwargs):
        print 'starting client ' + str(idx+1)
        client_processes.append(Popen(**client_kwarg))
        sleep(0.5)

    def kill_server():
        server_process.kill()
        print 'server killed'

    server_timer = Timer(12 * 60, kill_server) # 12 minutes
    server_timer.start()
    server_process.wait()
    server_timer.cancel()

    errors = []
    killed = False
    if server_process.returncode != 0:
        killed = True
        print 'error: server'

finally:
    server_kwargs['stdout'].close()
    server_kwargs['stderr'].close()

for client_kwarg in client_kwargs:
    client_kwarg['stdout'].close()

sleep(1)
for client_process in client_processes:
    if client_process.poll() is None:
        client_process.kill()
    if client_process.returncode != 0:
        print 'error: client' 

if killed:
    winner = '-1'
else:
    winner = open(path.join('server', 'result'), 'r').read()
    print("raw winner: " + winner)


if winner == '-1':
    print 'Result: tie'

elif winner[0:3] == '909':
    for idx in range(self.num_players):
        if idx == int(winner[3:]):
            print 'Winner: ' + idx
        
elif winner[0:2] == 's:':
    print 'Acho que quer dizer que o jogo nao tem 2 players, so um score absoluto: Score: ' + winner[2:]
    print 'Olhar codigo do game.py do Yggdrasil e procurar por \'elif winner[0:2] == \'s:\''    
else:
    print 'Unknown result: ' + winner
