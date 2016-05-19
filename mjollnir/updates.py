"""
This script contains actions that should be executed after a 'git pull',
so that the virtual machine's environment is properly updated.
"""

## Imports and constants ##

import json
import os
import shutil
import sys

from glob import glob
from os import path
from subprocess import CalledProcessError, check_output

MJOLLNIR_SOLUTIONS = path.expanduser("~/mjollnir-solutions")
MJOLLNIR = "/Mjollnir/mjollnir/"
BIFROST = "/Mjollnir/bifrost/"
VIGRIDRSRC = "/Mjollnir/vigridr/src/"

sys.path.append(VIGRIDRSRC)
from change_game_code import change_game_code
sys.path.pop(-1)

## Local functions ##

def _changed(filename_or_foldername, local):
    """
    Checks if the argument was changed.
    Parameters:
        filename_or_foldername - a string, the name of the item to check
        local - a boolean,
                if True: Looks for changes on current branch, comparing the current stage state and the last commit state
                if False: Looks for changes on master branch, comparing the last commit state and the last but one commit state
    Returns:
        a bool - whether it was changed
    """
    if local:
        return check_output(["git", "diff", "--staged", "--name-only", filename_or_foldername]) != ""
    else:
        return check_output(["git", "diff", "--name-only", "master@{1}", "master", filename_or_foldername]) != ""

def _mjollnir_solutions_folder_changed(mjollnir):
    guessnumber = "/Mjollnir/vigridr/src/games/guessnumber/"
    if path.isdir(guessnumber):
      shutil.rmtree(guessnumber)
      
    if mjollnir.VERSION == "0.1":
        target_hash = 282429717669259069 # Calculated from the uploaded VM image

        text = ""
        for solution in sorted(glob(path.join(MJOLLNIR_SOLUTIONS, "*", "*", "*.*"))):
            text += open(solution).read()

        if hash(text) == target_hash:
            shutil.rmtree(MJOLLNIR_SOLUTIONS)
            mjollnir.create(["tictactoe", "cpp", "random"])
            mjollnir.create(["tron", "cs", "random"])
            mjollnir.create(["wumpus", "py", "random"])
            mjollnir.create(["go", "py", "random"])
            os.remove(path.expanduser("~/location"))

## Exported functions ##

def update(mjollnir, local=False):
    build_game = mjollnir._build_game
    build_solution = mjollnir.build
    logger = mjollnir.logger

    # Silence invocations of build_game and build_solution
    mjollnir.logger = mjollnir._SilentLogger()

    try:
        logger.info("Checking for configuration changes...")
        something_changed = False
        dev_null = open(os.devnull, "w")

        _mjollnir_solutions_folder_changed(mjollnir)

        if _changed(VIGRIDRSRC, local):
            logger.info(" * Source code for game(s) changed")
            something_changed = True
            if local:
                logger.info("   -> Rebuilding all changed games binaries (on current branch, since last commit)")
            else:
                logger.info("   -> Rebuilding all changed games binaries (comparing, on master, last commit with last but one commit)")
            for game in glob(path.join(VIGRIDRSRC, "games", "*")):
                if not path.isdir(game):
                    continue
                if not json.load(open(path.join(game, "config.json"), "r"))["published"]:
                    continue
                game_name = path.basename(game)
                if _changed(path.join(VIGRIDRSRC, "games", game_name, ""), local):
                    try:
                        logger.info("      Game '%s'..." % game_name)
                        build_game(game_name, stdout=dev_null)
                    except CalledProcessError as e:
                        logger.warn("Failure to build game '%s'" % game_name)
                        print str(e)
                        # If game failed, doesn't matter. Go to next.

            bin_folders = glob(path.expanduser(path.join("~", "mjollnir-solutions", "*", "*", "bin")))
            if bin_folders:
                logger.info("   -> Rebuilding solution binaries")
                for bin_folder in bin_folders:
                    solution_folder = path.dirname(bin_folder)
                    solution_name = path.basename(solution_folder)
                    game_name = path.basename(path.dirname(solution_folder))
                    if _changed(path.join(VIGRIDRSRC, "games", game_name, ""), local):
                        logger.info("      Solution '%s/%s'..." % (game_name, solution_name))
                        os.chdir(solution_folder)
                        if build_solution([], stdout=dev_null) != 0:
                            logger.warn("Failure to build solution '%s'" % solution_name)
                            # If solution failed, doesn't matter. Go to next.

        if _changed(path.join(MJOLLNIR, "autocomplete-mjollnir"), local) or _changed(path.join(MJOLLNIR, "include-mjollnir"), local):
            logger.info(" * A shell script was changed.")
            logger.info("   -> In order to get full capabilities, please either")
            logger.info("      close and reopen your terminals or run '. ~/.bashrc' in each of them.")
            something_changed = True

        if not something_changed:
            logger.info("Nothing to do.")

    except KeyboardInterrupt as e:
        logger.err(repr(e))
        return 1

    finally:
        os.chdir(VIGRIDRSRC)
        change_game_code("template", copy_sample_clients=True, copy_tests=False, copy_obj=False, used_logger=mjollnir._SilentLogger())
        mjollnir.logger = logger
        dev_null.close()

    return 0

__all__ = ["update"]

