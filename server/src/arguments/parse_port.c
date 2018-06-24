/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** parse_port
*/

#include "server.h"
#include <ctype.h>

bool parse_port(size_t ac, const char **av, params_t *params, size_t *i)
{
	*i += 1;
	if (*i >= ac)
		return (false);
	for (size_t j = 0; av[*i][j]; ++j)
		if (!isdigit(av[*i][j]))
			return (false);
	params->port = strtoul(av[*i], 0, 10);
	return (params->port > 0 && params->port <= 65535);
}
