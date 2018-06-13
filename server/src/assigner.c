/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

static tuple_t fcmd[] = {
	{"forward",     &cmd_forward},
	{"right",       &cmd_right},
	{"left",        &cmd_left},
	{"look",        &cmd_look},
	{"inventory",   &cmd_inventory},
	{"broadcast",   &cmd_broadcast},
	{"connect_nbr", &cmd_connect_nbr},
	{"fork",        &cmd_fork},
	{"eject",       &cmd_eject},
	{"take",        &cmd_take},
	{"set",         &cmd_set},
	{"incantation", &cmd_incantation}
};

static void cmd_unknown(client_t *cl, cmd_t *cmd)
{
	dprintf(2, "[%s] Unknown command %s\n", cl->ip, cmd->name);
	llist_push(cl->pending, 1, strdup("ko\n"));
}

void clear_cmd(cmd_t *cmd)
{
	if (cmd && cmd->param) {
		for (int i = 0; i < cmd->nparam && cmd->param[i]; ++i)
			free(cmd->param[i]);
		free(cmd->param);
		free(cmd);
	}
}

void show_cmd(cmd_t *cmd)
{
	printf("CMD: %s\n", cmd->name);
	for (int i = 0; i < cmd->nparam; ++i)
		printf("[%d] %s\n", i, cmd->param[i]);
}

void proceed_cmd(control_t *ctrl, client_t *cl)
{
	(void) ctrl;
	cmd_t *cmd = ((cmd_t *) cl->cmd->head->payload);

	show_cmd(cmd);
	for (int i = 0; i < CMD_COUNT; ++i)
		if (!strcasecmp(cmd->name, fcmd[i].cmd)) {
			fcmd[i].func(ctrl, cl);
			break;
		} else if (i + 1 == CMD_COUNT)
			cmd_unknown(cl, cmd);
	clear_cmd(llist_remove(cl->cmd, 0));
}