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

static void cmd_unknown(control_t *ctrl, client_t *cl)
{
	(void)ctrl;
	dprintf(2, "[%s] Unknown command %s\n", cl->ip, cl->cmd.cmd);
}

void proceed_cmd(control_t *ctrl, client_t *cl)
{
	(void)ctrl;
	printf("CMD: %s\n", cl->cmd.name);
	for (int i = 0; i < cl->cmd.nparam; ++i)
		printf("[%d] %s\n", i, cl->cmd.param[i]);
	//TODO free_params();
}