/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** elem
*/

#include "object.h"

elem_t *lobj_elem_init(char *type, void *data)
{
	elem_t *elem = malloc(sizeof(elem_t));
	char *type_str = strdup(type);

	if (elem == NULL || type_str == NULL)
		return (NULL);
	elem->type = type_str;
	elem->data = data;
	return (elem);
}

void lobj_free_elem(elem_t *elem, bool free_payload)
{
	free(elem->type);
	if (free_payload)
		free(elem->data);
	free(elem);
}
