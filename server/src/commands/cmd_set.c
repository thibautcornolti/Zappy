/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

void cmd_set(control_t *control, client_t *client)
{
	// TODO: Add argument passing field to task_t
	(void)(control);
	client->task.time = 7;
	client->task.type = SET;
}

void exec_set(control_t *control, client_t *client)
{
	// TODO: Drop item
	(void)(control);
	client->task.type = NONE;
}
