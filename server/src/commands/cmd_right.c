/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

void cmd_right(control_t *control, client_t *client)
{
	(void)(control);
	client->task.time = 7;
	client->task.type = RIGHT;
	llist_push(client->pending, 1, strdup(OK_MSG));
}

void exec_right(control_t *control, client_t *client)
{
	(void)(control);
	if (client->facing == WEST)
		client->facing = NORTH;
	else
		client->facing += 1;
}
