/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

void cmd_incantation(control_t *control, client_t *client)
{
	// TODO: Check incantation requirements
	(void)(control);
	client->task.time = 300;
	client->task.type = INCANTATION;
}

void exec_incantation(control_t *control, client_t *client)
{
	(void)(control);
	(void)(client);
}
