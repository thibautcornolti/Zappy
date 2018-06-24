/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"
#include <regex.h>

static bool reset_cast(control_t *ctrl, client_t *cl, size_t id)
{
	for (list_elem_t *it = ctrl->clients->head; it; it = it->next) {
		cl = it->payload;
		if (cl->id == id) {
			cl->task.time = 1;
			return (true);
		}
	}
	return (false);
}

void adm_cast(control_t *ctrl, client_t *cl)
{
	regex_t preg;
	cmd_t *cmd = cl->cmd->head->payload;
	char pattern[] = R"(^cast[[:space:]]+[[:digit:]]+[[:space:]]*$)";
	char usage[] = "USAGE: CAST: [id]";

	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE | REG_EXTENDED);
	if (cmd->nparam == 1 && !regexec(&preg, cmd->cmd, 0, NULL, 0) &&
		reset_cast(ctrl, cl, strtoul(cmd->param[0], 0, 10)))
		add_pending(cl, strdup("Command [CAST] successful"));
	else
		add_pending(cl, strdup(usage));
	regfree(&preg);
}