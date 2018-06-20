/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include <regex.h>
#include "server.h"

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
	char pattern[] = R"(?:cast \d+$)";
	char usage[] = "CAST: [id]";

	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE);
	if (cmd->nparam == 3 && !regexec(&preg, cmd->cmd, 0, NULL, 0) &&
	    reset_cast(ctrl, cl, strtoul(cmd->param[0], 0, 10)))
		add_pending(cl, "Command [SPAWN] successful");
	else
		add_pending(cl, strdup(usage));
	regfree(&preg);
}