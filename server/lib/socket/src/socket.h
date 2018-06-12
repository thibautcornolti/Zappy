/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** socket
*/

#pragma once

#ifndef CHECK
#define CHECK(x, y, z)                                                        \
	if ((x)y) {                                                           \
		dprintf(2, "[%s:%d]: %s\n", __FILE__, __LINE__,               \
			__FUNCTION__);                                        \
		return (z);                                                   \
	}
#endif

int create_server(short port);
