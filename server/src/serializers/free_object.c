/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** free_object
*/

#include "server.h"

void free_object(void *ctx, char *key, elem_t *value)
{
	(void)(ctx);
	(void)(key);
	free_elem(value, true);
}

void free_elem_obj(object_t *object)
{
	lobj_for_each(
		object, (void (*)(void *, char *, elem_t *))(free_object), 0);
	lobj_destroy(object);
}
