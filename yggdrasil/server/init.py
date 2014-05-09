#!/usr/bin/python

import json
import logging
import signal
import sys
import time
from flask import Flask, request
from logging.handlers import TimedRotatingFileHandler 
from manager import run

app = Flask(__name__)
handler = TimedRotatingFileHandler('/Mjollnir/yggdrasil/server/logs/server.log', when='midnight', backupCount=7)
logger = logging.getLogger('werkzeug')

logger.setLevel(logging.INFO)
logger.addHandler(handler)
app.logger.setLevel(logging.INFO)
app.logger.addHandler(handler)

@app.route("/run")
def handler():
    uid1 = request.args.get('uid1')
    uid2 = request.args.get('uid2')
    pid = request.args.get('pid')
    
    if not uid1:
        return json.dumps({
            'status': 'error',
            'error': 'missing uid1'
        })

    if not uid2:
        return json.dumps({
            'status': 'error',
            'error': 'missing uid2'
        })
    
    if not pid:
        return json.dumps({
            'status': 'error',
            'error': 'missing pid'
        })

    response = json.dumps(run(uid1, uid2, pid))
    logger.info(('(%s, %s, %s) => ' % (uid1, uid2, pid)) + response)
    return response

@app.errorhandler(404)
def error_handler(error):
    return '404'

@app.errorhandler(500)
def error_handler(error):
    return json.dumps({
        'status': 'error',
        'error': 'unexpected'
    })

def signal_handler(signal, frame):
    logger.warn('=== Process stopped (%d) at %s ===' % (signal, time.strftime('%H:%M:%S')))
    sys.exit(-1)
    
if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)

    app.run(host='0.0.0.0', port='30403')
