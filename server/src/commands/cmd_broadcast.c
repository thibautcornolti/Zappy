/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"
#include <math.h>
#include <stdio.h>

static const float bounds[][2] = {[1] = {1.249046, 1.892547},
	[2] = {1.892547, 2.819842},
	[3] = {2.819842, 3.463343},
	[4] = {3.463343, 4.390638},
	[5] = {4.390638, 5.034139},
	[6] = {5.034139, 5.961434},
	[7] = {5.961434, 6.604936},
	[8] = {6.604936, 7.532231}};

void cmd_broadcast(control_t *control, client_t *client)
{
	cmd_t *cmd = client->cmd->head->payload;

	(void)(control);
	client->task.time = 7;
	client->task.type = BROADCAST;
	client->task.data = (cmd->nparam != 0) ? strdup(cmd->cmd + 10) : 0;
}

float get_angle(control_t *control, vec2_t pos1, vec2_t pos2)
{
	float x;
	float y;
	float norm;
	float angle;

	x = (float)(pos1.x) - (float)(pos2.x);
	x = (x > control->params.width) ? -x : x;
	y = (float)(pos1.y) - (float)(pos2.y);
	y = (y > control->params.height) ? -y : y;
	norm = sqrtf(x * x + y * y);
	if (norm == 0)
		return (1.3);
	x /= norm;
	y /= norm;
	angle = atan2f(y, x);
	if (angle < 1.249046)
		angle += 2 * M_PI;
	return (angle);
}

void send_message(control_t *control, client_t *client, char *message)
{
	client_t *cl;
	float angle;
	char *str = 0;

	for (list_elem_t *it = control->clients->head; it; it = it->next) {
		cl = it->payload;
		if (cl->state != PLAYER || cl->fd == client->fd)
			continue;
		angle = get_angle(control, client->pos, cl->pos);
		for (size_t i = 1; i < 9; ++i)
			if (angle >= bounds[i][0] && angle <= bounds[i][1]) {
				i += 2 * cl->facing;
				i %= 8;
				i += 1;
				asprintf(&str, "message %lu, %s", i, message);
				add_pending(cl, str);
				break;
			}
	}
}

void exec_broadcast(control_t *control, client_t *client)
{
	client->task.type = NONE;
	if (client->task.data == 0) {
		add_pending(client, strdup("ko"));
		return;
	}
	send_message(control, client, client->task.data);
	event_broadcast(control, client, client->task.data);
	add_pending(client, strdup("ok"));
	free(client->task.data);
	client->task.data = 0;
}
