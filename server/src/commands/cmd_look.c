/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "item_names.h"

static const long increments[][2] = {[NORTH] = {0, 1},
	[SOUTH] = {0, -1},
	[EAST] = {1, 0},
	[WEST] = {-1, 0}};

static const long sidesteps[][2] = {[NORTH] = {1, 0},
	[SOUTH] = {-1, 0},
	[EAST] = {0, 1},
	[WEST] = {0, -1}};

void cmd_look(control_t *control, client_t *client)
{
	(void)(control);
	client->task.time = 7;
	client->task.type = LOOK;
}

static char *concat_names(
	control_t *control, char *acc, item_t item, size_t idx)
{
	(void)(control);
	if (idx)
		acc = lstr_append(acc, " ");
	return (lstr_append(acc, item_names[item]));
}

static char *concat_players(
	vec2_t *pos, char *acc, client_t *client, size_t idx)
{
	(void)(idx);
	if (client->pos.x != pos->x || client->pos.y != pos->y)
		return (acc);
	if (lstr_equals(acc, "") == false)
		acc = lstr_append(acc, " ");
	return (lstr_append(acc, "player"));
}

static char *list_items(control_t *control, vec2_t pos)
{
	list_t *list = map_get(control, pos.x, pos.y);
	char *ret = llist_reduce(list,
		(void *(*)(void *, void *, void *, size_t))(concat_names),
		control, strdup(""));

	if (lstr_equals(ret, ""))
		ret = lstr_append(ret, " ");
	ret = llist_reduce(control->clients,
		(void *(*)(void *, void *, void *, size_t))(concat_players),
		&pos, ret);
	return (ret);
}

void exec_look(control_t *control, client_t *client)
{
	const long *axis = increments[client->facing];
	const long *sidestep = sidesteps[client->facing];
	long radius = 0;
	char *list = strdup("[");
	char *tmp;
	vec2_t pos;

	for (long i = 0; i <= (long)(client->level); ++i) {
		for (long j = -radius; j <= radius; ++j) {
			pos.x = (((long)(client->pos.x)) + axis[0] * i +
					sidestep[0] * j +
					control->params.width) %
				control->params.width;
			pos.y = (((long)(client->pos.y)) + axis[1] * i +
					sidestep[1] * j +
					control->params.height) %
				control->params.height;
			dprintf(2, "POS: {%lu,%lu}\n", pos.x, pos.y);
			tmp = list_items(control, pos);
			if (i != 0 || j != 0)
				list = lstr_append(list, ",");
			dprintf(2, "TMP: '%s'\n", tmp);
			list = lstr_append(list, tmp);
			free(tmp);
		}
		radius += 1;
	}
	list = lstr_append(list, "]");
	client->task.type = NONE;
	add_pending(client, list);
}
