/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include <regex.h>
#include "server.h"

static facing_t orient[] = {
	['N'] = NORTH,
	['E'] = EAST,
	['S'] = SOUTH,
	['W'] = WEST
};

void adm_face(control_t *ctrl, client_t *cl)
{
	regex_t preg;
	cmd_t *cmd = cl->cmd->head->payload;
	char pattern[] = R"(^face[[:space:]][[:digit:]]+[[:space:]][NESW]*$)";
	char usage[] = "USAGE: FACE [id] [N|E|S|W]";
	client_t *tmp;
	size_t id;

	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE | REG_EXTENDED);
	if (cmd->nparam == 2 && !regexec(&preg, cmd->cmd, 0, NULL, 0)) {
		id = strtoul(cmd->param[0], 0, 10);
		tmp = llist_find(ctrl->clients, (bool (*)(void *, void *,
			size_t)) (find_client), &id);
		if (tmp && tmp->id == id) {
			tmp->facing = orient[toupper(*cmd->param[1])];
			add_pending(cl, strdup("Command [FACE] successful"));
		} else
			add_pending(cl, strdup(usage));
	} else
		add_pending(cl, strdup(usage));
	regfree(&preg);
}