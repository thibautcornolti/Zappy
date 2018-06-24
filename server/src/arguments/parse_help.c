/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** parse_help
*/

#include "server.h"

bool parse_help(size_t ac, const char **av, params_t *params, size_t *i)
{
	(void)(ac);
	(void)(av);
	(void)(i);
	params->help = true;
	return (true);
}
