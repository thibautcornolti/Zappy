/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by brinc
*/

#include "server.h"

void gui_map_size(control_t *ctrl, client_t *cl)
{
	object_t *ret = lobj_init();
	object_t *size = lobj_init();
	elem_t *elem;
	char *serialized;
	cmd_t *cmd = cl->cmd->head->payload;

	lobj_set(ret, "type", "string", strdup("response"));
	lobj_set(ret, "response-type", "string", strdup("map-size"));
	lobj_set(size, "width", "number",
		double_to_void_ptr(ctrl->params.width));
	lobj_set(size, "height", "number",
		double_to_void_ptr(ctrl->params.height));
	lobj_set(ret, "size", "object", size);
	elem = lobj_elem_init("object", ret);
	serialized = ljson_serialize(elem);
	llist_push(cl->pending, 1, serialized);
	free_elem(elem, false);
	free_json(cmd->json);
}
