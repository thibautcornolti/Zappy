/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** parse_names
*/

#include "server.h"

static const char *blacklist[] = {"admin", "gui", 0};

bool check_blacklist(const char *team)
{
	for (size_t i = 0; blacklist[i]; ++i)
		if (lstr_equals(team, blacklist[i])) {
			dprintf(2, "-n: Use of blacklisted team '%s'.\n",
				blacklist[i]);
			return (false);
		}
	return (true);
}

bool parse_names(size_t ac, const char **av, params_t *params, size_t *i)
{
	size_t s = *i;

	params->nteam = 0;
	while (1) {
		if (++s >= ac || lstr_starts_with(av[s], "-"))
			break;
		if (check_blacklist(av[s]) == false)
			return (false);
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