# PSU Network Unit - Zappy

## Introduction

This project is about playing a game.  
In this game, the goal is to reach level 8.  
To elevate to the next level, stones are required and sometimes multiple player must be present.  
Differents kinds and quantities of stone are required depending on the level to reach.  
Along with stones, food is required to live as they are consumed with time.  
Stones and food can be picked up from the ground or dropped to the ground.  
The game is implemented and exposed by the server.  
An artificial intelligence can connect to it and play the game.  
A GUI can connect to the server, access custom commands to hook into the state of the game and be notified of every events occuring in the game.  
An administrator can also connect to the server and access custom commands to alter the state of the game or the server configuration.  

## Build instructions

Here are the instructions on how to build the different modules of the project.

### Server

The server module is implemented in C.
It can be found in the `server/` folder.

No dependancy is required to build it except `gcc`.

To start the build, do either:
```bash
# From the repository root folder
make -C server

# From the server/ folder
make
```

To restart the build, use 'make re' instead of 'make'

You can then launch it like this:
```bash
# From the repository root folder
./server/zappy_server [-n [teams]...] [-f [ticks per second]] [-p [port]] [-h]

# From the server/ folder
./zappy_server [-n [teams]...] [-f [ticks per second]] [-p [port]] [-h]
```
