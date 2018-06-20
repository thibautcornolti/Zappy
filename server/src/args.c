/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** args
*/

#include "server.h"
#include <stdio.h>

static const param_parse_t tab[] = {
	{"-p", parse_port}, {"-x", parse_width},
	{"-y", parse_height}, {"-n", parse_names}, {"-c", parse_clients},
	{"-f", parse_freq}, {"-h", parse_help}, {"-help", parse_help},
	{"--help", parse_help}, {0, 0}
};

int disp_help(const char *prog_name)
{
	dprintf(1, HELP_MSG, prog_name);
	return (0);
}

bool parse_args(size_t ac, const char **av, params_t *params)
{
	size_t j = 0;

	for (size_t i = 1; i < ac; ++i) {
		j = 0;
		while (tab[j].cmd && strcmp(av[i], tab[j].cmd) != 0)
			j += 1;
		if (tab[j].cmd == 0) {
			dprintf(2, "%s: Unknown argument.\n", av[i]);
			return (false);
		}
		else if (tab[j].func(ac, av, params, &i) == false)
			return (false);
	}
	return (true);
}
