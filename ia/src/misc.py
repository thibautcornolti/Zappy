# coding = utf-8
import os
import subprocess
import src.config as config
from src.classes.ia_res.Ant import ant

process = None


def my_log(*args, **kwargs):
    status = "Slave"
    if ant.is_queen:
        status = "Queen"
    #print("[{}][{}]".format(os.getpid(), status), *args, **kwargs)
    pass


def my_print(*args, **kwargs):
    status = "Slave"
    if ant.is_queen:
        status = "Queen"
    print("[{}][{}]".format(os.getpid(), status), *args, **kwargs)


def dup_me():
    global process
    process = subprocess.Popen(
        [config.file_name, "-h", str(config.hostname), "-p", str(config.port), "-n", str(config.team_name)])
    my_print("Forked into ", process.pid)


def wait_son():
    global process
    if process:
        process.wait()
