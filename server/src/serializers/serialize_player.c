/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** serialize_player
*/

#include "server.h"

void *double_to_void_ptr(double d)
{
	return *(void **)(&d);
}

object_t *serialize_player(client_t *client)
{
	object_t *ret = lobj_init();
	object_t *pos = lobj_init();

	lobj_set(ret, "type", "string", strdup("player"));
	lobj_set(ret, "id", "number", double_to_void_ptr(client->fd));
	lobj_set(pos, "x", "number", double_to_void_ptr(client->pos.x));
	lobj_set(pos, "y", "number", double_to_void_ptr(client->pos.y));
	lobj_set(ret, "pos", "object", pos);
	return (ret);
}
