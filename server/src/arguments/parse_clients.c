/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** parse_clients
*/

#include "server.h"
#include <ctype.h>

bool parse_clients(size_t ac, const char **av, params_t *params, size_t *i)
{
	*i += 1;
	if (*i >= ac)
		return (false);
	for (size_t j = 0; av[*i][j]; ++j)
		if (!isdigit(av[*i][j]))
			return (false);
	params->nclt = strtoul(av[*i], 0, 10);
	return (params->nclt > 0);
}
