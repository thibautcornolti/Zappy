/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

vec2_t move(control_t *control, vec2_t v, long x, long y)
{
	v.x += control->params.width + x;
	v.y += control->params.height + y;
	v.x %= control->params.width;
	v.y %= control->params.height;
	return (v);
}

void move_forward(control_t *control, client_t *client)
{
	long x = 0;
	long y = 0;

	x = ((client->facing == EAST) ? 1 : x);
	x = ((client->facing == WEST) ? -1 : x);
	y = ((client->facing == NORTH) ? -1 : y);
	y = ((client->facing == SOUTH) ? 1 : y);
	client->pos = move(control, client->pos, x, y);
	event_player_move(control, client);
}

void move_directed(control_t *control, client_t *client, facing_t direction)
{
	long x = 0;
	long y = 0;

	x = ((direction == EAST) ? 1 : x);
	x = ((direction == WEST) ? -1 : x);
	y = ((direction == NORTH) ? -1 : y);
	y = ((direction == SOUTH) ? 1 : y);
	client->pos = move(control, client->pos, x, y);
	event_player_move(control, client);
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
