/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

void cmd_left(control_t *control, client_t *client)
{
	(void)(control);
	client->task.time = 7;
	client->task.type = LEFT;
}

void exec_left(control_t *control, client_t *client)
{
	(void)(control);
	if (client->facing == EAST)
		client->facing = NORTH;
	else
		client->facing += 1;
	llist_push(client->pending, 1, strdup(OK_MSG));
	client->task.type = NONE;
}
