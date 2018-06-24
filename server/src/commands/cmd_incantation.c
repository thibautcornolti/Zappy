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

static void incantation_success(
	control_t *control, client_t *client, list_t *count)
{
	for (size_t i = 1; i < ITEM_COUNT; ++i)
		for (size_t c = 0; c < required[client->level][i]; ++c)
			map_remove(control, client->pos.x, client->pos.y, i);
	llist_for_each(count,
		(void (*)(void *, void *, size_t))(upgrade_level), control);
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

static void helper_cmd_incantation(control_t *control, client_t *client,
	void (*event)(control_t *, client_t *), char *message)
{
	add_pending(client, strdup(message));
	event(control, client);
}

void cmd_incantation(control_t *ctrl, client_t *cl)
{
	list_t *count;

	cl->task.time = 300;
	cl->task.type = INCANTATION;
	if (cl->level == 8) {
		add_pending(cl, strdup(KO_MSG));
		return;
	}
	count = validate_clients(ctrl, cl);
	if (count == 0)
		return;
	llist_destroy(count);
	for (size_t i = 1; i < ITEM_COUNT; ++i)
		if (count_items(ctrl, cl->pos, i) != required[cl->level][i]) {
			helper_cmd_incantation(
				ctrl, cl, event_incantation_fail, KO_MSG);
			return;
		}
	helper_cmd_incantation(
		ctrl, cl, event_incantation_start, ELEVATION_MSG);
}

void exec_incantation(control_t *ctrl, client_t *cl)
{
	list_t *count;

	cl->task.type = NONE;
	if (cl->level == 8) {
		add_pending(cl, strdup(KO_MSG));
		return;
	}
	count = validate_clients(ctrl, cl);
	if (count == 0)
		return;
	for (size_t i = 1; i < ITEM_COUNT; ++i)
		if (count_items(ctrl, cl->pos, i) != required[cl->level][i]) {
			add_pending(cl, strdup(KO_MSG));
			llist_destroy(count);
			event_incantation_fail(ctrl, cl);
			return;
		}
	incantation_success(ctrl, cl, count);
}
