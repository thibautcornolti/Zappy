/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

static tuple_t fcmd[] = {
	{"forward", &cmd_forward}, {"right", &cmd_right},
	{"left", &cmd_left}, {"look", &cmd_look},
	{"inventory", &cmd_inventory}, {"broadcast", &cmd_broadcast},
	{"connect_nbr", &cmd_connect_nbr}, {"fork", &cmd_fork},
	{"eject", &cmd_eject}, {"take", &cmd_take}, {"set", &cmd_set},
	{"incantation", &cmd_incantation}
};

static void cmd_unknown(control_t *ctrl, client_t *cl)
{
	(void)ctrl;
	dprintf(2, "[%s] Unknown command %s\n", cl->ip, cl->cmd.name);
}

void clear_cmd(cmd_t *cmd)
{
	if (cmd->param) {
		for (int i = 0; i < cmd->nparam && cmd->param[i]; ++i)
			free(cmd->param[i]);
		free(cmd->param);
	}
	cmd->param = 0;
	cmd->nparam = 0;
	memset(cmd->cmd, 0, CMD_SIZE);
	memset(cmd->name, 0, CMD_SIZE);
}

void proceed_cmd(control_t *ctrl, client_t *cl)
{
	(void)ctrl;
	printf("CMD: %s\n", cl->cmd.name);
	for (int i = 0; i < cl->cmd.nparam; ++i)
		printf("[%d] %s\n", i, cl->cmd.param[i]);
	for (int i = 0; i < CMD_COUNT; ++i)
		if (!strcasecmp(cl->cmd.name, fcmd[i].cmd)) {
			fcmd[i].func(ctrl, cl);
			break;
		}
		else if (i + 1 == CMD_COUNT)
			cmd_unknown(ctrl, cl);
	clear_cmd(&cl->cmd);
}