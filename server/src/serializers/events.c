/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** events
*/

#include "server.h"

static void send_message(char *message, client_t *client, size_t idx)
{
	(void)(idx);
	if (client->state == GUI)
		add_pending(client, strdup(message));
}

void forward_event(control_t *control, object_t *message)
{
	elem_t *elem = lobj_elem_init("object", message);
	char *serialized = ljson_serialize(elem);

	free_elem(elem, false);
	llist_for_each(control->clients,
		(void (*)(void *, void *, size_t))(send_message), serialized);
	free(serialized);
}
