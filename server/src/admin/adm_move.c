/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include <regex.h>
#include "server.h"

static bool move_player(control_t *ctrl, client_t *cl, size_t y, size_t x)
{
	bool ret = false;
	size_t id = strtoul(((cmd_t *)cl->cmd->head->payload)->param[0], 0, 10);

	for (size_t i = 0; i < ctrl->clients->length; ++i) {
		if (((client_t *)(llist_at(ctrl->clients, i)))->id == id){
			((client_t *)(llist_at(ctrl->clients, i)))->pos.x = x;
			((client_t *)(llist_at(ctrl->clients, i)))->pos.y = y;
			ret = true;
			break;
		}
	}
	return (ret);
}

void adm_move(control_t *ctrl, client_t *cl)
{
	regex_t preg;
	cmd_t *cmd = cl->cmd->head->payload;
	char pattern[] = R"(?:\/move \d+ \d+ \d+$)";
	char usage[] = "MOVE: [id] [y] [x]";
	size_t y, x;

	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE);
	if (cmd->nparam == 3 && !regexec(&preg, cmd->cmd, 0, NULL, 0)) {
		y = strtoul(cmd->param[1], 0, 10);
		x = strtoul(cmd->param[2], 0, 10);
		if (y < ctrl->params.height && x < ctrl->params.width
		    && move_player(ctrl, cl, y, x)) {}
			llist_push(cl->pending, 1,
			           "Command [SPAWN] successful");
	} else
		llist_push(cl->pending, 1, strdup(usage));
	regfree(&preg);
}
