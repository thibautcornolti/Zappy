/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** event_player_death
*/

#include "server.h"

void event_player_death(control_t *control, client_t *client, char *reason)
{
	object_t *ret = lobj_init();
	object_t *data = lobj_init();

	lobj_set(ret, "type", "string", strdup("event"));
	lobj_set(ret, "event-type", "string", strdup("player-death"));
	lobj_set(data, "id", "number", double_to_void_ptr(client->id));
	lobj_set(data, "reason", "string", strdup(reason));
	lobj_set(ret, "data", "object", data);
	forward_event(control, ret);
}