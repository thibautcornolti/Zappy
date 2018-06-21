/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

static const size_t required[][7] = {[0] = {1, 0, 0, 0, 0, 0, 0},
	[1] = {1, 1, 0, 0, 0, 0, 0},
	[2] = {2, 1, 1, 1, 0, 0, 0},
	[3] = {2, 2, 0, 1, 0, 2, 0},
	[4] = {4, 1, 1, 2, 0, 1, 0},
	[5] = {4, 1, 2, 1, 3, 0, 0},
	[6] = {6, 1, 2, 3, 0, 1, 0},
	[7] = {6, 2, 2, 2, 2, 2, 1}};

void cmd_incantation(control_t *control, client_t *client)
{
	(void)(control);
	client->task.time = 300;
	client->task.type = INCANTATION;
}

static bool *same_level(
	client_t *client, bool *acc, client_t *elem, size_t idx)
{
	(void)(idx);
	*acc = *acc && (client->level == elem->level);
	return (acc);
}

static void incantation_success(
	control_t *control, client_t *client, list_t *count)
{
	for (size_t i = 1; i < ITEM_COUNT; ++i)
		for (size_t c = 0; c < required[client->level][i]; ++c)
			map_remove(control, client->pos.x, client->pos.y, i);
	llist_for_each(
		count, (void (*)(void *, void *, size_t))(upgrade_level), 0);
	llist_destroy(count);
}

static list_t *validate_clients(control_t *control, client_t *client)
{
	list_t *count = count_clients(control, client->pos);
	bool ret = true;

	llist_reduce(count,
		(void *(*)(void *, void *, void *, size_t))(same_level),
		client, &ret);
	if (count->length != required[client->level][0] || ret == false) {
		add_pending(client, strdup(KO_MSG));
		llist_destroy(count);
		return (0);
	}
	return (count);
}

void exec_incantation(control_t *control, client_t *client)
{
	list_t *count;

	client->task.type = NONE;
	if (client->level == 8) {
		add_pending(client, strdup(KO_MSG));
		return;
	}
	count = validate_clients(control, client);
	if (count == 0)
		return;
	for (size_t i = 1; i < ITEM_COUNT; ++i)
		if (count_items(control, client->pos, i) !=
			required[client->level][i]) {
			add_pending(client, strdup(KO_MSG));
			llist_destroy(count);
			return;
		}
	incantation_success(control, client, count);
}
