import os
import sys
import shutil
from subprocess import check_call

VIGRIDR_SRC = '/Mjollnir/vigridr/src'
TEST_SRC = '/Mjollnir/vigridr/localdev'
### To compile, just change the next 4 values ###
CURRENT_GAME = 'tron'
CLIENT_EXTENSION = 'cpp'
SERVER_EXTENSION = 'cpp'

sys.path.append(VIGRIDR_SRC)
from change_game_code import change_game_code
sys.path.pop(-1)

class Logger():
    def error(self, msg):
        print msg
    def info(self, msg):
        print msg

os.chdir(VIGRIDR_SRC)
change_game_code(CURRENT_GAME, False, True, False, Logger())

idx = 1

print('######################## Compiling ########################')

try:
	shutil.rmtree(os.path.join(TEST_SRC, 'client1'))
	shutil.rmtree(os.path.join(TEST_SRC, 'client2'))
	shutil.rmtree(os.path.join(TEST_SRC, 'server'))
except:
	pass

for x in range(0, 2):
	print('======================== Setting up client ' + str(idx) + ' ========================')
	shutil.copy(os.path.join(VIGRIDR_SRC, 'games', CURRENT_GAME, 'sampleclient', 'ClientLogic.' + CLIENT_EXTENSION), os.path.join(VIGRIDR_SRC, 'client', 'ClientLogic.' + CLIENT_EXTENSION))

	print('======================== Running make client' + CLIENT_EXTENSION + ' ========================')
	os.chdir('..')
	check_call(['make', 'client' + CLIENT_EXTENSION])

	print('======================== Copying client files ========================')
	shutil.copytree(os.path.join('bin', CLIENT_EXTENSION), os.path.join(TEST_SRC, 'client' + str(idx)))	

	print('======================== Running make remove ========================')
	check_call(['make', 'remove'])
	os.chdir(VIGRIDR_SRC)

	idx = idx + 1

print('======================== Running make server ========================')
check_call(['make', 'server'])

print('======================== Copying server files ========================')
os.chdir(TEST_SRC)
os.mkdir('server')
shutil.copy(os.path.join(VIGRIDR_SRC, '..', 'bin', SERVER_EXTENSION, 'server'), os.path.join(TEST_SRC, 'server', 'server'))
shutil.copy(os.path.join(VIGRIDR_SRC, '..', 'bin', SERVER_EXTENSION, 'server'), os.path.join(VIGRIDR_SRC, 'games', CURRENT_GAME, 'bin'))

print('======================== Making tests ========================')
os.chdir(VIGRIDR_SRC)
os.chdir('..')
check_call(['make', 'test'])

print('######################## Finished ########################')
