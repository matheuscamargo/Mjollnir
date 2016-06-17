
__all__ = ['run', 'kill', 'games', 'compile']

import json
import logging
import threading
import time
from flask import Flask, jsonify
from dbmanager import get_game_names
from game import Compiler, Game
from logging.handlers import TimedRotatingFileHandler
from Queue import Empty, Queue
from time import sleep
from uuid import uuid4

KILL = 'KILL'
COMPILE = 'COMPILE'
RUN = 'RUN'

def now():
    return time.strftime('%H:%M:%S')

class GamesManager(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.logger = logging.getLogger('yggdrasil')
        self.logger.setLevel(logging.INFO)
        self.logger.addHandler(TimedRotatingFileHandler('/Mjollnir/yggdrasil/server/logs/manager.log', when='midnight', backupCount=7))
        self.games_queue = Queue()
        self.completed_queue = Queue()
        self.running = True
        self.game_names = get_game_names()

    def run(self):
        try:
            self.logger.info('[%s] === Starting game queue ===' % (now(), ))
            while self.running:
                siids, uids, cid, tid, mid, type = self.games_queue.get() # wait for a requested game

                if type == RUN:
                    pid = self.game_names[cid]
                    self.logger.info('[%s] Starting game %s in %s' % (now(), siids, pid))
                    with Game(siids, uids, cid, pid, tid, mid, self.logger) as game:
                        game.download()
                        game.compile()
                        game.run()
                        game.upload()
                        self.completed_queue.put_nowait(game.result)
                    self.logger.info('[%s] Ended game %s in %s' % (now(), siids, pid))
                elif type == COMPILE:
                    pid = self.game_names[cid]
                    # When using the COMPILE flag, the first argument is the sid
                    sid = siids
                    self.logger.info('[%s] Starting compilation of %s' % (now(), sid))
                    with Compiler(sid, pid, self.logger) as compiler:
                        compiler.download()
                        compiler.compile()
                        compiler.upload()
                    self.logger.info('[%s] Ended compilation of %s' % (now(), sid))
                else: # KILL
                    break
                
            self.logger.info('[%s] === Game queue stopped  ===' % (now(), ))
        except Exception as e:
            self.running = False
            self.logger.info('[%s] === Game queue interrupted ===\n%s' % (now(), str(e)))

    def run_game(self, siids, uids, cid, tid):

        if not self.is_alive() or not self.running:
            return jsonify({
                'status': 'error',
                'error': 'Game thread not running',
                'mid': 0
            })

        if cid not in self.game_names:
            self.logger.warn("[%s] cid %s doesn't exist" % (now(), cid))
            return jsonify({
                'status': 'error',
                'error': "cid %s doesn't exist" % (cid,),
                'mid': 0
            })

        pid = self.game_names[cid]

        try:
            mid = str(uuid4())
            self.games_queue.put((siids, uids, cid, tid, mid, RUN))
            self.logger.info('[%s] Enqueued game %s in %s' % (now(), siids, pid))
            return jsonify({
                'status': 'ok',
                'mid': mid
            })
        except Exception as e:
            self.logger.info('[%s] Could not enqueue %s in %s\n%s' % (now(), siids, cid, str(e)))
            return jsonify({
                'status': 'error',
                'error': 'Could not enqueue the requested game',
                'mid': 0
            })

    def compile(self, sid, cid):
        if not self.is_alive() or not self.running:
            return {
                'status': 'error',
                'error': 'Compilation thread not running'
            }

        if cid not in self.game_names:
            self.logger.warn("[%s] cid %s doesn't exist" % (now(), cid))
            return {
                'status': 'error',
                'error': "cid %s doesn't exist" % (cid,)
            }

        try:
            self.games_queue.put((sid, None, cid, None, None, COMPILE))
            self.logger.info('[%s] Enqueued compilation of %s' % (now(), sid))
            return {
                'status': 'ok'
            }
        except Exception as e:
            self.logger.info('[%s] Could not enqueue compilation of %s\n%s' % (now(), sid, str(e)))
            return {
                'status': 'error',
                'error': 'Could not enqueue the requested compilation'
            }

    def kill(self):
        self.running = False
        self.games_queue.put((None, None, None, None, None, KILL))
        
    def games(self):
        completed = []
        try:
            while True:
                completed.append(self.completed_queue.get_nowait())
        except Empty:
            pass
        return completed


manager = GamesManager()
manager.start()

def run(siids, uids, cid, tid = None):
    return manager.run_game(siids, uids, cid, tid)

def kill():
    manager.kill()

def games():
    return manager.games()

def compile(sid, cid):
    return manager.compile(sid, cid)
