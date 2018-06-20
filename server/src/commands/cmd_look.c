/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

static const long increments[][2] = {
	[NORTH] = {0,  1},
	[SOUTH] = {0, -1},
	[EAST] = { 1, 0},
	[WEST] = {-1, 0}
};

void cmd_look(control_t *control, client_t *client)
{
	(void)(control);
	client->task.time = 7;
	client->task.type = LOOK;
}

void exec_look(control_t *control, client_t *client)
{
	// TODO: Look around based on level
	(void)(control);
	for (size_t i = 0; i < client->level; ++i) {
	}
	client->task.type = NONE;
	llist_push(client->pending, 1, strdup(OK_MSG));
}
