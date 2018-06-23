/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** event_egg_hatch
*/

#include "server.h"

void event_egg_hatch(control_t *control, egg_t *egg)
{
	object_t *ret = lobj_init();
	object_t *data = lobj_init();

	lobj_set(ret, "type", "string", strdup("event"));
	lobj_set(ret, "event-type", "string", strdup("egg-hatch"));
	lobj_set(data, "team", "string", strdup(egg->team->name));
	lobj_set(data, "pos", "object", serialize_position(egg->pos));
	lobj_set(ret, "data", "object", data);
	forward_event(control, ret);
}
