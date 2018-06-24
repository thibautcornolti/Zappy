/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by brinc
*/

#include "server.h"

static bool str_isnum(const char *str)
{
	for (size_t i = 0; str[i]; ++i)
		if (isdigit(str[i]) == 0)
			return (false);
	return (str[0] != 0);
}

static bool convert_to_number(size_t *ret, elem_t *elem)
{
	if (lstr_equals(elem->type, "number"))
		*ret = *((double *)(&elem->data));
	else if (lstr_equals(elem->type, "string") && str_isnum(elem->data))
		*ret = strtoul(elem->data, 0, 10);
	else
		return (false);
	return (true);
}

static bool get_coords(vec2_t *v, object_t *root)
{
	elem_t *elem = lobj_get(root, "pos");
	elem_t *tmp;

	if (elem == 0 || lstr_equals(elem->type, "object") == false)
		return (false);
	tmp = lobj_get(elem->data, "x");
	if (!tmp || convert_to_number(&v->x, tmp) == false)
		return (false);
	tmp = lobj_get(elem->data, "y");
	if (!tmp || convert_to_number(&v->y, tmp) == false)
		return (false);
	return (true);
}

void gui_tile(control_t *control, client_t *cl)
{
	cmd_t *cmd = cl->cmd->head->payload;
	vec2_t pos = {0, 0};
	object_t *ret;

	if (get_coords(&pos, cmd->json->root->data) == false ||
		pos.x >= control->params.width ||
		pos.y >= control->params.height)
		ret = emit_command_error("Position is out of map bounds.");
	else {
		ret = lobj_init();
		lobj_set(ret, "type", "string", strdup("response"));
		lobj_set(ret, "response-type", "string", strdup("tile"));
		lobj_set(ret, "pos", "object", serialize_position(pos));
		lobj_set(ret, "data", "list", create_tile_data(control, pos));
	}
	finalize_json(cl, ret, cmd);
}
