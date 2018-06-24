/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** event_incantation_start
*/

#include "server.h"

void event_incantation_start(control_t *control, client_t *client)
{
	object_t *ret = lobj_init();
	object_t *data = lobj_init();

	lobj_set(ret, "type", "string", strdup("event"));
	lobj_set(ret, "event-type", "string", strdup("incantation-start"));
	lobj_set(data, "id", "number", double_to_void_ptr(client->id));
	lobj_set(data, "current-level", "number",
		double_to_void_ptr(client->level));
	lobj_set(ret, "data", "object", data);
	forward_event(control, ret);
}
