/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"
#include <math.h>
#include <stdio.h>

static const float bounds[][3] = {{1, -1.892547, -1.249046},
	{2, -2.819842, -1.892547}, {3, -M_PI, -2.819842}, {3, 2.819842, M_PI},
	{4, 1.892547, 2.819842}, {5, 1.249046, 1.892547},
	{6, 0.321751, 1.249046}, {7, 0, 0.321751}, {7, -0.321751, 0},
	{8, -1.249046, -0.321751}, {0, 0, 0}};

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

	x = (float)(pos1.x) - (float)(pos2.x);
	x = (x > (control->params.width / 2)) ? -x : x;
	y = (float)(pos1.y) - (float)(pos2.y);
	y = (y > (control->params.height / 2)) ? -y : y;
	norm = sqrtf(x * x + y * y);
	if (norm == 0)
		return (0);
	x /= norm;
	y /= norm;
	return (atan2f(y, x));
}

void do_thing(
	control_t *control, client_t *client, client_t *cl, char *message)
{
	float angle;
	char *str = 0;

	if (cl->pos.x == client->pos.x && cl->pos.y == client->pos.y) {
		asprintf(&str, "message 0, %s", message);
		add_pending(cl, str);
		return;
	}
	angle = get_angle(control, client->pos, cl->pos);
	for (size_t i = 0; bounds[i][0]; ++i)
		if (angle >= bounds[i][1] && angle <= bounds[i][2]) {
			i = bounds[i][0];
			i += 2 * cl->facing;
			i %= 8;
			i = (i == 0) ? 8 : i;
			asprintf(&str, "message %lu, %s", i, message);
			add_pending(cl, str);
			break;
		}
}

void send_message(control_t *control, client_t *client, char *message)
{
	client_t *cl;

	for (list_elem_t *it = control->clients->head; it; it = it->next) {
		cl = it->payload;
		if (cl->state != PLAYER || cl->fd == client->fd)
			continue;
		do_thing(control, client, cl, message);
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
