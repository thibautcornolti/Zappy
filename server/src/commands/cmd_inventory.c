/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"
#include "str/src/str.h"

void cmd_inventory(control_t *control, client_t *client)
{
	(void)(control);
	client->task.type = INVENTORY;
	client->task.time = 1;
}

void exec_inventory(control_t *control, client_t *client)
{
	char *str = 0;

	(void)(control);
	asprintf(&str,
		"[food %lu, linemate %lu, deraumere %lu, sibur %lu, "
		"mendiane %lu, phiras %lu, thystame %lu]",
		client->inventory[FOOD], client->inventory[LINEMATE],
		client->inventory[DERAUMERE], client->inventory[SIBUR],
		client->inventory[MENDIANE], client->inventory[PHIRAS],
		client->inventory[THYSTAME]);
	add_pending(client, str);
	client->task.type = NONE;
}
