/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** socket
*/

#pragma once

#ifndef CHECK
	#define CHECK(x, y, z) if ((x) == (y)) return (z)
#endif

int create_server(short port);
