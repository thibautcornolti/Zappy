/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include <regex.h>
#include "server.h"

void adm_killall(control_t *ctrl, client_t *cl)
{
	regex_t preg;
	cmd_t *cmd = cl->cmd->head->payload;
	char pattern[] = R"(^killall[[:space:]]*$)";
	char usage[] = "USAGE: KILLALL";

	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE | REG_EXTENDED);
	if (!cmd->nparam && !regexec(&preg, cmd->cmd, 0, NULL, 0)) {
		add_pending(cl, strdup("Command [KILLALL] successful"));
	}
	else
		add_pending(cl, strdup(usage));
	regfree(&preg);
}
