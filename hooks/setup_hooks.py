#!/usr/bin/env python

import os
import shutil
import argparse

HOOKS_PATH = os.path.dirname(os.path.abspath(__file__))

def parse():
  parser = argparse.ArgumentParser()
  parser.add_argument("--clean", action="store_true", help="Remove hooks")
  args = parser.parse_args()

  git_folder = os.path.join("..",".git")

  if not os.path.isdir(git_folder):
    with open(git_folder) as f:
      line = f.readline()
      line = line.split(" ")
    git_folder = os.path.join("..", line[1][:-1])

  git_folder = os.path.join(git_folder, "hooks")

  if args.clean:
    clean(git_folder)
  else:
    setup_hooks(git_folder)

def setup_hooks(git_folder):
  for hook in os.listdir(HOOKS_PATH):
    if hook not in __file__:
      shutil.copy("{}/{}".format(HOOKS_PATH, hook), os.path.join(git_folder,  hook))

def clean(git_folder):
  for hook in os.listdir(HOOKS_PATH):
    if hook not in __file__:
      os.remove(os.path.join(git_folder, hook))

if __name__ == "__main__":
  parse()
