/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "item_names.h"
#include "str/src/str.h"

void cmd_set(control_t *control, client_t *client)
{
	cmd_t *cmd = client->cmd->head->payload;

	(void)control;
	client->task.time = 7;
	client->task.type = TAKE;
	client->task.data = ((cmd->nparam != 1) ? 0 : strdup(cmd->param[0]));
}

void exec_set(control_t *control, client_t *client)
{
	item_t chosen = ITEM_COUNT;

	client->task.type = NONE;
	if (client->task.data == 0) {
		add_pending(client, strdup(KO_MSG));
		return;
	}
	for (size_t i = 0; i < ITEM_COUNT; ++i)
		if (lstr_equals(client->task.data, item_names[i])) {
			chosen = i;
			break;
		}
	if (chosen == ITEM_COUNT) {
		add_pending(client, strdup(KO_MSG));
		return;
	}
	if (client->inventory[chosen] != 0) {
		map_add(control, client->pos.x, client->pos.y, chosen);
		client->inventory[chosen] -= 1;
		add_pending(client, strdup(OK_MSG));
		event_item_drop(control, client, chosen);
	} else
		add_pending(client, strdup(KO_MSG));
	free(client->task.data);

}
