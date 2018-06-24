/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** parser_helper
*/

#include "server.h"

void add_param(cmd_t *cmd, char *str)
{
	cmd->param = realloc(cmd->param, sizeof(char *) * (cmd->nparam + 1));
	if (cmd->param) {
		cmd->param[cmd->nparam] = strdup(str);
		cmd->nparam += 1;
	}
}
