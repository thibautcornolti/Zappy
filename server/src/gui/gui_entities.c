/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by brinc
*/

#include "item_names.h"

static object_t *create_item_entry(
	control_t *control, list_t *data, item_t item)
{
	object_t *ret = lobj_init();

	(void)(control);
	lobj_set(ret, "type", "string", strdup(item_names[item]));
	lobj_set(ret, "data", "list", data);
	return (ret);
}

static void list_items(control_t *control, list_t **data, vec2_t pos)
{
	size_t count;
	object_t *ret;

	for (size_t i = 0; i < ITEM_COUNT; i += 1) {
		count = count_items(control, pos, i);
		if (count != 0) {
			ret = serialize_position(pos);
			lobj_set(ret, "amount", "number",
				double_to_void_ptr(count));
			llist_push(data[i], 1, lobj_elem_init("object", ret));
		}
	}
}

static object_t *list_clients(control_t *control)
{
	object_t *ret = lobj_init();
	list_t *data = llist_init();
	client_t *client;

	lobj_set(ret, "type", "string", strdup("player"));
	for (list_elem_t *it = control->clients->head; it; it = it->next) {
		client = it->payload;
		if (client->state == PLAYER)
			llist_push(data, 1,
				lobj_elem_init(
					"object", serialize_player(client)));
	}
	lobj_set(ret, "data", "list", data);
	return (ret);
}

void gui_entities(control_t *control, client_t *client)
{
	object_t *ret = lobj_init();
	list_t **data = calloc(ITEM_COUNT, sizeof(list_t *));
	list_t *list = llist_init();
	vec2_t pos = {0, 0};

	lobj_set(ret, "type", "string", strdup("response"));
	lobj_set(ret, "response-type", "string", strdup("entities"));
	for (size_t i = 0; i < ITEM_COUNT; i += 1)
		data[i] = llist_init();
	for (pos.y = 0; pos.y < control->params.height; pos.y += 1)
		for (pos.x = 0; pos.x < control->params.width; pos.x += 1)
			list_items(control, data, pos);
	for (size_t i = 0; i < ITEM_COUNT; i += 1)
		llist_push(list, 1,
			lobj_elem_init("object",
				create_item_entry(control, data[i], i)));
	free(data);
	llist_push(list, 1, lobj_elem_init("object", list_clients(control)));
	lobj_set(ret, "data", "list", list);
	finalize_json(client, ret, client->cmd->head->payload);
}
