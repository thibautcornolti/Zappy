/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

void cmd_fork(control_t *control, client_t *client)
{
	(void)(control);
	client->task.time = 42;
	client->task.type = FORK;
}

void exec_fork(control_t *control, client_t *client)
{
	egg_t *egg = calloc(1, sizeof(egg_t));
	static size_t id = 0;

	egg->id = id++;
	egg->delay = 600;
	egg->team = client->team;
	egg->pos = client->pos;
	llist_push(control->eggs, 1, egg);
	add_pending(client, strdup(OK_MSG));
	event_egg_drop(control, client, egg);
	client->task.type = NONE;
}
