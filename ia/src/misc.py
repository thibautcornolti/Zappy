# coding = utf-8
import os
import subprocess
import src.config as config

process = None

def my_print(*args, **kwargs):
    print("[{}]".format(os.getpid()), *args, **kwargs)

def dup_me():
    global process
    process = subprocess.run([config.file_name, "-h", config.hostname, "-p", config.port, "-n", config.team_name])

def wait_son():
    pass