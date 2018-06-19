/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** cmd_incantation_helper
*/

#include "server.h"

static list_t *count_client(
	client_t *client, list_t *acc, client_t *cl, size_t idx)
{
	bool is_valid =
		(cl->pos.x == client->pos.x) && (cl->pos.y == client->pos.y);

	(void)(idx);
	if (is_valid)
		llist_push(acc, 1, cl);
	return (acc);
}

list_t *count_clients(control_t *control, client_t *client)
{
	return (llist_reduce(control->clients,
		(void *(*)(void *, void *, void *, size_t))(count_client),
		client, llist_init()));
}

static size_t count_item(item_t *item, size_t acc, item_t elem, size_t idx)
{
	(void)(idx);
	return (acc + (*item == elem));
}

size_t count_items(control_t *control, vec2_t pos, item_t item)
{
	return ((size_t)(llist_reduce(map_get(control, pos.x, pos.y),
		(void *(*)(void *, void *, void *, size_t))(count_item), &item,
		0)));
}

void upgrade_level(control_t *ctx, client_t *client, size_t idx)
{
	(void)(ctx);
	(void)(idx);
	client->level += 1;
}

