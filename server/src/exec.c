/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** exec
*/

#include "server.h"

static void (*fcmd[])(control_t *, client_t *) = {
	[NONE] = 0,
	[FORWARD] = &exec_forward,
	[RIGHT] = &exec_right,
	[LEFT] = &exec_left,
	[LOOK] = &exec_look,
	[INVENTORY] = &exec_inventory,
	[BROADCAST] = &exec_broadcast,
	[CONNECT_NBR] = &exec_connect_nbr,
	[FORK] = &exec_fork,
	[EJECT] = &exec_eject,
	[TAKE] = &exec_take,
	[SET] = &exec_set,
	[INCANTATION] = &exec_incantation
};

bool exec_task(control_t *control, client_t *client)
{
	if (client->task.type == NONE)
		return (true);
	client->task.time -= 1;
	if (client->task.time != 0)
		return (true);
	fcmd[client->task.type](control, client);
	return (true);
}
