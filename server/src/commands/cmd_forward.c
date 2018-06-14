/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

void cmd_forward(control_t *control, client_t *client)
{
	(void)(control);
	client->task.time = 7;
	client->task.type = FORWARD;
	llist_push(client->pending, 1, strdup(OK_MSG));
}

void exec_forward(control_t *control, client_t *client)
{
	long x = 0;
	long y = 0;

	x = ((client->facing == EAST) ? 1 : x);
	x = ((client->facing == WEST) ? -1 : x);
	y = ((client->facing == NORTH) ? -1 : y);
	y = ((client->facing == SOUTH) ? 1 : y);
	client->pos.x += x;
	client->pos.y += y;
	client->pos.x %= control->params.width;
	client->pos.y %= control->params.height;
}
