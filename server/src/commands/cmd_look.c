/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

void cmd_look(control_t *control, client_t *client)
{
	(void)(control);
	client->task.time = 7;
	client->task.type = LOOK;
}

void exec_look(control_t *control, client_t *client)
{
	// TODO: Look around based on level
	(void)(control);
	client->task.type = NONE;
	llist_push(client->pending, 1, strdup(OK_MSG));
}
