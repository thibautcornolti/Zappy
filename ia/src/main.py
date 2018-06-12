#!/usr/bin/env python3.5

import argparse
from src.classes import AI, Client


def parse_arg():
    parser = argparse.ArgumentParser(
        usage="USAGE: ./zappy_ai -p port -n name -h machine", add_help=False)
    parser.add_argument("-help", action="store_true")
    parser.add_argument("-p", help="is the port number", required=True,
                        type=int)
    parser.add_argument("-n", help="is the name of the team", required=True)
    parser.add_argument("-h",
                        help="is the name of the machine; localhost by default",
                        default="localhost")

    args = parser.parse_args()
    if args.help:
        exit("""USAGE: ./zappy_ai -p port -n name -h machine
    port    is the port number
    name    is the name of the team
    machine is the name of the machine; localhost by default""")
    return args


def run(args):
    with Client(args.h, args.p) as (client, error):
        if error:
            exit(error)
        ai = AI(client, args.n)
        print(ai.ulapid)
        print(ai.broadcast("test"))
        print(client.read_line())


def main():
    args = parse_arg()
    try:
        run(args)
    except Exception as e:
        exit("Fatal Error: %s" % e)


if __name__ == "__main__":
    main()
