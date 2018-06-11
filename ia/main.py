#!/usr/bin/env python3.5

import argparse

# USAGE: ./zappy_ai -p port -n name -h machine
# port is the port number
# name is the name of the team
# machine is the name of the machine; localhost by default

def parse_arg():
    parser = argparse.ArgumentParser(usage="USAGE: ./zappy_ai -p port -n name -h machine", add_help=False)
    parser.add_argument("-help", action="store_true")
    parser.add_argument("-p", help="is the port number", required=True)
    parser.add_argument("-n", help="is the name of the team", required=True)
    parser.add_argument("-h", help="is the name of the machine; localhost by default", default="localhost")

    args = parser.parse_args()
    if args.help:
        exit("""USAGE: ./zappy_ai -p port -n name -h machine
    port    is the port number
    name    is the name of the team
    machine is the name of the machine; localhost by default""")
    return args


def main():
    args = parse_arg()


if __name__ == "__main__":
    main()
