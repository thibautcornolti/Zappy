/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** parse_names
*/

#include "server.h"

bool parse_names(size_t ac, const char **av, params_t *params, size_t *i)
{
	size_t s = *i;

	params->nteam = 0;
	while (1) {
		s += 1;
		if (s >= ac)
			break;
		params->nteam += 1;
	}
	if (params->nteam == 0) {
		dprintf(2, "-n: No teams have been declared.\n");
		return (false);
	}
	CHECK(params->teams = calloc(params->nteam, sizeof(char *)), == 0,
		false);
	for (size_t s = 0; s < params->nteam; ++s)
		CHECK(params->teams[s] = strdup(av[*i + s + 1]), == 0, false);
	*i += s;
	return (true);
}