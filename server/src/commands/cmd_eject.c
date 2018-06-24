/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

static const facing_t opposite[] = {
	[NORTH] = SOUTH, [SOUTH] = NORTH, [EAST] = WEST, [WEST] = EAST};

size_t compute_direction(facing_t facing, facing_t direction)
{
	return (abs((long)(facing) - (long)(direction)) * 2 + 1);
}

void cmd_eject(control_t *control, client_t *client)
{
	(void)(control);
	client->task.type = NONE;
	client->task.time = 7;
}

void exec_eject(control_t *control, client_t *client)
{
	client_t *cl;
	char *str = 0;

	for (list_elem_t *it = control->clients->head; it; it = it->next) {
		cl = it->payload;
		if (cl->fd != client->fd && cl->pos.x == client->pos.x &&
			cl->pos.y == client->pos.y) {
			move_directed(control, cl, client->facing);
			asprintf(&str, "eject: %lu",
				compute_direction(
					cl->facing, opposite[client->facing]));
			add_pending(cl, str);
		}
	}

	client->task.type = NONE;
}
