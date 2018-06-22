/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include <regex.h>
#include "server.h"

void lvlup_player(size_t id)
{
	(void)id;
}

void adm_lvlup(control_t *ctrl, client_t *cl)
{
	(void)ctrl;
	regex_t preg;
	cmd_t *cmd = cl->cmd->head->payload;
	char pattern[] = R"(^lvlup[[:space:]]+[[:digit:]]+[[:space:]]*$)";
	char usage[] = "USAGE: LVLUP: [id]";

	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE | REG_EXTENDED);
	if (cmd->nparam == 1 && !regexec(&preg, cmd->cmd, 0, NULL, 0)) {
		lvlup_player(strtoul(cmd->param[0], 0, 10));
		add_pending(cl, strdup("Command [LVLUP] successful"));
	}
	else
		add_pending(cl, strdup(usage));
	regfree(&preg);
}