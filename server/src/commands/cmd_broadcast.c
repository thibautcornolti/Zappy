/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

void cmd_broadcast(control_t *control, client_t *client)
{
	(void)(control);
	(void)(client);
}

void exec_broadcast(control_t *control, client_t *client)
{
	(void)(control);
	client->task.type = NONE;
}
