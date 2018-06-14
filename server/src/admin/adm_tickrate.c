/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include <regex.h>
#include "server.h"

void adm_tickrate(control_t *ctrl, client_t *cl)
{
	regex_t preg;
	cmd_t *cmd = cl->cmd->head->payload;
	char pattern[] = R"(?:\/tickrate \d+$)";
	char usage[] = "TICKRATE: [tickrate]";

	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE);
	if (cmd->nparam == 1 && !regexec(&preg, cmd->cmd, 0, NULL, 0)) {
		ctrl->params.tickrate = strtoul(cmd->param[0], 0, 10);
		llist_push(cl->pending, 1, "Command [TICKRATE] successful");
	} else
		llist_push(cl->pending, 1, strdup(usage));
	regfree(&preg);
}