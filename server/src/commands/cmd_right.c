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
}

void exec_right(control_t *control, client_t *client)
{
	(void)(control);
	if (client->facing == NORTH)
		client->facing = EAST;
	else
		client->facing -= 1;
	add_pending(client, strdup(OK_MSG));
	event_player_turn(control, client);
	client->task.type = NONE;
}
