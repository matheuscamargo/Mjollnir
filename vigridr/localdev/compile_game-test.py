import os
import sys
import shutil
from subprocess import check_call

VIGRIDR_SRC = '/Mjollnir/vigridr/src'
TEST_SRC = '/Mjollnir/vigridr/localdev'
CURRENT_GAME = 'go'
PLAYER_LOGIC_FILES = ['Player1Logic.cpp', 'Player2Logic.cpp']
EXTENSION = 'cpp'
LANGUAGE = 'cpp'

sys.path.append(VIGRIDR_SRC)
from change_game_code import change_game_code
sys.path.pop(-1)

class Logger():
    def error(self, msg):
        print msg
    def info(self, msg):
        print msg

os.chdir(VIGRIDR_SRC)
change_game_code(CURRENT_GAME, False, False, False, Logger())

idx = 1

print('######################## Compiling ########################')

try:
	shutil.rmtree(os.path.join(TEST_SRC, 'client1'))
	shutil.rmtree(os.path.join(TEST_SRC, 'client2'))
	shutil.rmtree(os.path.join(TEST_SRC, 'server'))
except:
	pass

for player_logic_file in PLAYER_LOGIC_FILES:
	print('======================== Setting up client ' + str(idx) + ' ========================')
	shutil.copy(os.path.join(TEST_SRC, player_logic_file), os.path.join(VIGRIDR_SRC, 'client', 'ClientLogic.' + EXTENSION))

	print('======================== Running make client' + EXTENSION + ' ========================')
	os.chdir('..')
	check_call(['make', 'client' + EXTENSION])

	print('======================== Copying client files ========================')
	shutil.copytree(os.path.join('bin', LANGUAGE), os.path.join(TEST_SRC, 'client' + str(idx)))	

	print('======================== Running make remove ========================')
	check_call(['make', 'remove'])
	os.chdir(VIGRIDR_SRC)

	idx = idx + 1

print('======================== Running make server ========================')
check_call(['make', 'server'])

print('======================== Copying server files ========================')
os.chdir(TEST_SRC)
os.mkdir('server')
shutil.copy(os.path.join(VIGRIDR_SRC, '..', 'bin', EXTENSION, 'server'), os.path.join(TEST_SRC, 'server', 'server'))
shutil.copy(os.path.join(VIGRIDR_SRC, '..', 'bin', EXTENSION, 'server'), os.path.join(VIGRIDR_SRC, 'games', CURRENT_GAME, 'bin'))

print('######################## Finished ########################')
