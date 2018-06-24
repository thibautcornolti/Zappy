/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** event_player_turn
*/

#include "server.h"

static const char *facings[] = {
	[NORTH] = "N", [SOUTH] = "S", [WEST] = "W", [EAST] = "E"};

void event_player_turn(control_t *control, client_t *client)
{
	object_t *ret = lobj_init();
	object_t *data = lobj_init();

	lobj_set(ret, "type", "string", strdup("event"));
	lobj_set(ret, "event-type", "string", strdup("player-turn"));
	lobj_set(data, "id", "number", double_to_void_ptr(client->id));
	lobj_set(data, "facing", "string", strdup(facings[client->facing]));
	lobj_set(ret, "data", "object", data);
	forward_event(control, ret);
}