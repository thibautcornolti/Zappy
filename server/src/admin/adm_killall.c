/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"
#include <regex.h>

void adm_killall(control_t *ctrl, client_t *cl)
{
	regex_t preg;
	cmd_t *cmd = cl->cmd->head->payload;
	char pattern[] = R"(^killall[[:space:]]*$)";
	char usage[] = "USAGE: KILLALL";
	client_t *tmp;

	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE | REG_EXTENDED);
	if (!cmd->nparam && !regexec(&preg, cmd->cmd, 0, NULL, 0)) {
		for (size_t i = 0; i < ctrl->clients->length;) {
			tmp = llist_at(ctrl->clients, i);
			if (tmp->state == PLAYER)
				evict_client(
					ctrl, llist_remove(ctrl->clients, i));
			else
				i += 1;
		}
		add_pending(cl, strdup("Command [KILLALL] successful"));
	} else
		add_pending(cl, strdup(usage));
	regfree(&preg);
}
