/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** free_json
*/

#include "server.h"

static const char *types[] = {
	"string", "number", "object", "boolean", "list", "null", 0};

static void (*funcs[])(void *value) = {(void (*)(void *))(free_elem_str), 0,
	(void (*)(void *))(free_elem_obj), 0,
	(void (*)(void *))(free_elem_list), 0, 0};

void free_elem_str(char *str)
{
	free(str);
}

void free_elem(elem_t *elem, bool free_only_data)
{
	for (size_t i = 0; types[i]; ++i)
		if (funcs[i] && lstr_equals(elem->type, types[i])) {
			funcs[i](elem->data);
			break;
		}
	if (free_only_data != true)
		lobj_free_elem(elem, false);
}

void free_json(json_t *json)
{
	free(json->raw);
	free(json->original);
	free_elem(json->root, false);
	free(json);
}
