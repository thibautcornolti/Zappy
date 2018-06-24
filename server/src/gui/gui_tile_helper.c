/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** gui_tile_helper
*/

#include "item_names.h"

void create_item_list(control_t *control, list_t *data, vec2_t pos)
{
	object_t *tmp;
	size_t count;

	for (size_t i = 0; i < ITEM_COUNT; ++i) {
		count = count_items(control, pos, i);
		if (count) {
			tmp = lobj_init();
			lobj_set(tmp, "type", "string", strdup(item_names[i]));
			lobj_set(tmp, "amount", "number",
				double_to_void_ptr(count));
			llist_push(data, 1, lobj_elem_init("object", tmp));
		}
	}
}

void create_player_list(control_t *control, list_t *data, vec2_t pos)
{
	list_t *clients = count_clients(control, pos);
	object_t *tmp = clients->length ? lobj_init() : 0;
	list_t *ids = clients->length ? llist_init() : 0;

	if (clients->length == 0)
		return;
	lobj_set(tmp, "type", "string", strdup("player"));
	for (list_elem_t *it = clients->head; it; it = it->next)
		llist_push(ids, 1,
			lobj_elem_init("number",
				double_to_void_ptr(
					((client_t *)(it->payload))->id)));
	lobj_set(tmp, "ids", "list", ids);
	llist_push(data, 1, lobj_elem_init("object", tmp));
	llist_destroy(clients);
}

list_t *create_tile_data(control_t *control, vec2_t pos)
{
	list_t *data = llist_init();

	create_item_list(control, data, pos);
	create_player_list(control, data, pos);
	return (data);
}
