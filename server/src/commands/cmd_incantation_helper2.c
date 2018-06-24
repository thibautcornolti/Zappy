/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** cmd_incantation_helper2
*/

#include "server.h"

bool *same_level(client_t *client, bool *acc, client_t *elem, size_t idx)
{
	(void)(idx);
	*acc = *acc && (client->level == elem->level);
	return (acc);
}
