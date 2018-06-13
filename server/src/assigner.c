/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

static tuple_t fcmd[] = {
		{"msv", &cmd_msv}, {"bct", &cmd_bct}, {"mct", &cmd_mct},
		{"tna", &cmd_tna}, {"ppo", &cmd_ppo}, {"plv", &cmd_plv},
		{"pin", &cmd_pin}, {"sgt", &cmd_sgt}, {"sst", &cmd_sst}
};

static void cmd_unknown(client_t *cl, cmd_t *cmd)
{
	dprintf(2, "[%s] Unknown command %s\n", cl->ip, cmd->name);
}

void clear_cmd(cmd_t *cmd)
{
	if (cmd->param) {
		for (int i = 0; i < cmd->nparam && cmd->param[i]; ++i)
			free(cmd->param[i]);
		free(cmd->param);
	}
	free(cmd);
}

void proceed_cmd(control_t *ctrl, client_t *cl)
{
	(void)ctrl;
	cmd_t *cmd = ((cmd_t *)cl->cmd->head->payload);

	printf("CMD: %s\n", cmd->name);
	for (int i = 0; i < cmd->nparam; ++i)
		printf("[%d] %s\n", i, cmd->param[i]);
	for (int i = 0; i < CMD_COUNT; ++i)
		if (!strcasecmp(cmd->name, fcmd[i].cmd)) {
			fcmd[i].func(ctrl, cl);
			break;
		} else if (i + 1 == CMD_COUNT)
			cmd_unknown(cl, cmd);
	clear_cmd(llist_remove(cl->cmd, 0));
}