/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** serialize_player
*/

#include "server.h"

static const char *facings[] = {
	[NORTH] = "N",
	[SOUTH] = "S",
	[WEST] = "W",
	[EAST] = "E"
};

void *double_to_void_ptr(double d)
{
	return *(void **)(&d);
}

object_t *serialize_player(client_t *client)
{
	object_t *ret = lobj_init();

	lobj_set(ret, "type", "string", strdup("player"));
	lobj_set(ret, "id", "number", double_to_void_ptr(client->id));
	lobj_set(ret, "pos", "object", serialize_position(client->pos));
	return (ret);
}
