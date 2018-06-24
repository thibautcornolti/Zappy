/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** consume
*/

#include "server.h"

bool consume_food(control_t *control)
{
	ssize_t size = ((ssize_t)(control->clients->length)) - 1;
	client_t *client;

	for (ssize_t i = size; i >= 0; i -= 1) {
		client = llist_at(control->clients, i);
		if (client->state != PLAYER)
			continue;
		client->food_delay -= (client->food_delay != 0);
		if (client->food_delay == 0) {
			client->inventory[FOOD] -=
				(client->inventory[FOOD] != 0);
			client->food_delay = FOOD_DELAY;
		}
		if (client->inventory[FOOD] == 0) {
			free_player(control, client);
			llist_remove(control->clients, i);
		}
	}
	return (true);
}

void consume_eggs(control_t *control)
{
	ssize_t size = ((ssize_t)(control->eggs->length)) - 1;
	egg_t *egg;

	for (ssize_t i = size; i >= 0; i -= 1) {
		egg = llist_at(control->eggs, i);
		egg->delay -= 1;
		if (egg->delay == 0) {
			egg->team->av += 1;
			egg->team->size += 1;
			egg->team->cl = realloc(egg->team->cl,
				egg->team->size * sizeof(client_t *));
			event_egg_hatch(control, egg);
			llist_remove(control->eggs, i);
		}
	}
}