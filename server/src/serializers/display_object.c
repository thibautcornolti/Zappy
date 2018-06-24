/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** display_object
*/

#include "server.h"

bool display_object(object_t *obj)
{
	elem_t *elem = lobj_elem_init("object", obj);
	char *serialized = ljson_serialize(elem);

	free(serialized);
	lobj_free_elem(elem, false);
	return (true);
}
