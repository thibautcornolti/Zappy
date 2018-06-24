/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"
#include "str/src/str.h"

void cmd_take(control_t *control, client_t *client)
{
	cmd_t *cmd = client->cmd->head->payload;

	(void)control;
	client->task.time = 7;
	client->task.type = TAKE;
	client->task.data = ((cmd->nparam != 1) ? 0 : strdup(cmd->param[0]));
}

void exec_take(control_t *control, client_t *client)
{
	item_t chosen = get_chosen_item(client);

	client->task.type = NONE;
	if (chosen == ITEM_COUNT) {
		add_pending(client, strdup(KO_MSG));
		return;
	}
	if (llist_includes(map_get(control, client->pos.x, client->pos.y),
		(void *)(chosen))) {
		map_remove(control, client->pos.x, client->pos.y, chosen);
		client->inventory[chosen] += 1;
		add_pending(client, strdup(OK_MSG));
		event_item_pickup(control, client, chosen);
	} else
		add_pending(client, strdup(KO_MSG));
	free(client->task.data);
}
