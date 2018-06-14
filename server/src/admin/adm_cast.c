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
	bool ret = false;

	for (size_t i = 0; i < ctrl->clients->length; ++i) {
		if (((client_t *) (llist_at(ctrl->clients, i)))->id == id) {
			((client_t *) (llist_at(ctrl->clients, i)))->task.time = 1;
			ret = true;
			break;
		}
	}
	return (ret);
}

void adm_cast(control_t *ctrl, client_t *cl)
{
	regex_t preg;
	cmd_t *cmd = cl->cmd->head->payload;
	char pattern[] = R"(?:\/cast \d+$)";
	char usage[] = "CAST: [id]";

	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE);
	if (cmd->nparam == 3 && !regexec(&preg, cmd->cmd, 0, NULL, 0) &&
	    reset_cast(ctrl, cl, strtoul(cmd->param[0], 0, 10)))
		llist_push(cl->pending, 1, "Command [SPAWN] successful");
	else
		llist_push(cl->pending, 1, strdup(usage));
	regfree(&preg);
}