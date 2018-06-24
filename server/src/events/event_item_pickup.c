/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** event_item_pickup
*/

#include "item_names.h"

void event_item_pickup(control_t *control, client_t *client, item_t item)
{
	object_t *ret = lobj_init();
	object_t *data = lobj_init();

	lobj_set(ret, "type", "string", strdup("event"));
	lobj_set(ret, "event-type", "string", strdup("item-pickup"));
	lobj_set(data, "id", "number", double_to_void_ptr(client->id));
	lobj_set(data, "item", "string", strdup(item_names[item]));
	lobj_set(data, "pos", "object", serialize_position(client->pos));
	lobj_set(ret, "data", "object", data);
	forward_event(control, ret);
}
