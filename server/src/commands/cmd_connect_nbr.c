/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"
#include "str/src/str.h"

void cmd_connect_nbr(control_t *control, client_t *client)
{
	(void)(control);
	llist_push(client->pending, 1,
		lstr_concat(strdup(""), 1, LSTR_INT, (int)(client->team->av)));
}

void exec_connect_nbr(control_t *control, client_t *client)
{
	(void)(control);
	client->task.type = NONE;
}
