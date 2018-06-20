/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"
#include <regex.h>

static bool move_player(control_t *ctrl, client_t *cl, size_t y, size_t x)
{
	cmd_t *cmd = cl->cmd->head->payload;
	size_t id = strtoul(cmd->param[0], 0, 10);

	for (list_elem_t *it = ctrl->clients->head; it; it = it->next) {
		cl = it->payload;
		if (cl->id == id) {
			cl->pos.x = x;
			cl->pos.y = y;
			return (true);
		}
	}
	return (false);
}

void adm_move(control_t *ctrl, client_t *cl)
{
	regex_t preg;
	cmd_t *cmd = cl->cmd->head->payload;
	char pattern[] = R"(?:move \d+ \d+ \d+$)";
	char usage[] = "MOVE: [id] [y] [x]";
	size_t y, x;

	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE);
	if (cmd->nparam == 3 && !regexec(&preg, cmd->cmd, 0, NULL, 0)) {
		y = strtoul(cmd->param[1], 0, 10);
		x = strtoul(cmd->param[2], 0, 10);
		if (y < ctrl->params.height && x < ctrl->params.width &&
			move_player(ctrl, cl, y, x)) {
		}
		add_pending(cl, "Command [SPAWN] successful");
	}
	else
		add_pending(cl, strdup(usage));
	regfree(&preg);
}
