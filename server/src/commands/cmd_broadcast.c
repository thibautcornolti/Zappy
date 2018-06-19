/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include <math.h>
#include "server.h"

void cmd_broadcast(control_t *control, client_t *client)
{
	(void)(control);
	client->task.time = 7;
	client->task.type = BROADCAST;
}

void exec_broadcast(control_t *control, client_t *client)
{
	client_t *cl;
	vec2f_t v;
	float norm;
	float angle;

	for (list_elem_t *it = control->clients->head; it; it = it->next) {
		cl = it->payload;
		if (cl->fd == client->fd)
			continue;
		v.x = (float)(client->pos.x) - (float)(cl->pos.x);
		v.y = (float)(client->pos.y) - (float)(cl->pos.y);
		norm = sqrtf(v.x * v.x + v.y * v.y);
		v.x /= norm;
		v.y /= norm;
		angle = atan2f(v.y, v.x);
	}
	client->task.type = NONE;
}
