/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** event_egg_drop
*/

#include "server.h"

void event_egg_drop(control_t *control, client_t *client, egg_t *egg)
{
	object_t *ret = lobj_init();
	object_t *data = lobj_init();

	lobj_set(ret, "type", "string", strdup("event"));
	lobj_set(ret, "event-type", "string", strdup("egg-drop"));
	lobj_set(data, "egg-id", "number", double_to_void_ptr(egg->id));
	lobj_set(data, "player-id", "number", double_to_void_ptr(client->id));
	lobj_set(data, "team", "string", strdup(client->team->name));
	lobj_set(data, "pos", "object", serialize_position(client->pos));
	lobj_set(ret, "data", "object", data);
	forward_event(control, ret);
}
