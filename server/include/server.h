/*
** EPITECH PROJECT, 2018
** PSU_zappy_2017
** File description:
** server.h
*/

#pragma once

#include "list/src/list.h"
#include "poll/src/common.h"
#include <netinet/in.h>
#include <stddef.h>

#define RBUFFER_SIZE 4096
#define CMD_SIZE 2048
#define CMD_COUNT 12

#ifndef CHECK
#define CHECK(x, y, z)                                                        \
	if ((x)y) {                                                           \
		dprintf(2, "[%s:%d]: %s\n", __FILE__, __LINE__,               \
			__FUNCTION__);                                        \
		return (z);                                                   \
	}
#endif

typedef struct rbuf_s {
	char buffer[RBUFFER_SIZE];
	int start;
	int end;
	int size;
} rbuf_t;

typedef struct cmd_s {
	char cmd[CMD_SIZE];
	char name[CMD_SIZE];
	char **param;
	int nparam;
} cmd_t;

typedef struct params_s {
	size_t port;
	size_t width;
	size_t height;
	char **teams;
	size_t nclt;
	size_t tickrate;
} params_t;
typedef struct vec2_s {
	size_t x;
	size_t y;
} vec2_t;

typedef struct client_s {
	int fd;
	char ip[INET_ADDRSTRLEN];
	list_t *cmd;
	rbuf_t rbuf;
	size_t id;
	vec2_t pos;
	size_t food;
	poll_t *node;
	list_t *pending;
} client_t;

typedef struct control_s {
	int fd;
	poll_t *list;
	list_t *clients;
} control_t;

typedef struct tuple_s {
	char *cmd;
	void (*func)(control_t *, client_t *);
} tuple_t;

//Comamnds
void cmd_forward(control_t *, client_t *);
void cmd_right(control_t *, client_t *);
void cmd_left(control_t *, client_t *);
void cmd_look(control_t *, client_t *);
void cmd_inventory(control_t *, client_t *);
void cmd_broadcast(control_t *, client_t *);
void cmd_connect_nbr(control_t *, client_t *);
void cmd_fork(control_t *, client_t *);
void cmd_eject(control_t *, client_t *);
void cmd_take(control_t *, client_t *);
void cmd_set(control_t *, client_t *);
void cmd_incantation(control_t *, client_t *);

size_t extract_rbuf_cmd(client_t *);
void proceed_cmd(control_t *, client_t *);
