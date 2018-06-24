/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include <regex.h>
#include "server.h"

static bool lvlup_player(control_t *ctrl, size_t id)
{
	bool ret = false;
	client_t *cl;

	for (size_t i = 0; i < ctrl->clients->length; ++i) {
		cl = llist_at(ctrl->clients, i);
		if (cl->id == id) {
			cl->level += 1;
			ret = true;
			break;
		}
	}
	return (ret);
}

void adm_lvlup(control_t *ctrl, client_t *cl)
{
	(void)ctrl;
	regex_t preg;
	cmd_t *cmd = cl->cmd->head->payload;
	char pattern[] = R"(^lvlup[[:space:]]+[[:digit:]]+[[:space:]]*$)";
	char usage[] = "USAGE: LVLUP: [id]";

	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE | REG_EXTENDED);
	if (cmd->nparam == 1 && !regexec(&preg, cmd->cmd, 0, NULL, 0) &&
		lvlup_player(ctrl, strtoul(cmd->param[0], 0, 10)))
			add_pending(cl, strdup("Command [LVLUP] successful"));
	else
		add_pending(cl, strdup(usage));
	regfree(&preg);
}