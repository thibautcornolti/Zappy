/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** event_player_join
*/

#include "server.h"

void event_player_join(control_t *control, client_t *client)
{
	object_t *ret = lobj_init();
	object_t *data = serialize_player(client);

	lobj_set(ret, "type", "string", strdup("event"));
	lobj_set(ret, "event-type", "string", strdup("player-join"));
	lobj_set(ret, "data", "object", data);
	forward_event(control, ret);
}
