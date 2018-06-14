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
	llist_push(client->pending, 1, strdup(OK_MSG));
}

void exec_fork(control_t *control, client_t *client)
{
	(void)(control);
	(void)(client);
}