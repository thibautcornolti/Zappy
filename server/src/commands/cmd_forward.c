/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

void move_forward(control_t *control, client_t *client)
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


void move_directed(control_t *control, client_t *client, facing_t direction)
{
	long x = 0;
	long y = 0;

	x = ((direction == EAST) ? 1 : x);
	x = ((direction == WEST) ? -1 : x);
	y = ((direction == NORTH) ? -1 : y);
	y = ((direction == SOUTH) ? 1 : y);
	client->pos.x += x;
	client->pos.y += y;
	client->pos.x %= control->params.width;
	client->pos.y %= control->params.height;
}

void cmd_forward(control_t *control, client_t *client)
{
	(void)(control);
	client->task.time = 7;
	client->task.type = FORWARD;
	add_pending(client, strdup(OK_MSG));
}

void exec_forward(control_t *control, client_t *client)
{
	move_forward(control, client);
	client->task.type = NONE;
}
