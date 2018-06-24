/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** parse_width
*/

#include "server.h"
#include <ctype.h>

bool parse_width(size_t ac, const char **av, params_t *params, size_t *i)
{
	*i += 1;
	if (*i >= ac)
		return (false);
	for (size_t j = 0; av[*i][j]; ++j)
		if (!isdigit(av[*i][j]))
			return (false);
	params->width = strtoul(av[*i], 0, 10);
	return (params->width > 0);
}
