/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** free_list
*/

#include "server.h"

void free_list(void *ctx, elem_t *elem, size_t idx)
{
	(void)(ctx);
	(void)(idx);
	free_elem(elem, false);
}

void free_elem_list(list_t *list)
{
	llist_for_each(list, (void (*)(void *, void *, size_t))(free_list), 0);
	llist_destroy(list);
}
