/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** display_object
*/

#include "server.h"
#include "json/src/json.h"

bool display_object(object_t *obj)
{
	elem_t *elem = lobj_elem_init("object", obj);
	char *serialized = ljson_serialize(elem);

	dprintf(2, "Player serialized:\n%s\n", serialized);
	free(serialized);
	lobj_free_elem(elem, false);
	return (true);
}
