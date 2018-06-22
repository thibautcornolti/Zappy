/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include <regex.h>
#include "item_names.h"
#include "server.h"

static bool add_item(control_t *ctrl, client_t *cl, size_t y, size_t x)
{
	int i = 0;
	bool ret = false;
	cmd_t *cmd = cl->cmd->head->payload;

	for(; i < ITEM_COUNT; ++i)
		if (!strcmp(item_names[i], cmd->param[0]))
			break;
	if (i < ITEM_COUNT) {
		map_add(ctrl, x, y, (item_t) i);
		ret = true;
	}
	return (ret);
}

void adm_spawn(control_t *ctrl, client_t *cl)
{
	regex_t preg;
	cmd_t *cmd = cl->cmd->head->payload;
	char pattern[] = R"(^spawn[[:space:]]+[[:alnum:]]+([[:space:]]+[[:digit:]]+){2}[[:space:]]*$)";
	char usage[] = "USAGE: SPAWN: [item] [y] [x]";
	size_t y, x;

	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE | REG_EXTENDED);
	if (cmd->nparam == 3 && !regexec(&preg, cmd->cmd, 0, NULL, 0)) {
		y = strtoul(cmd->param[1], 0, 10);
		x = strtoul(cmd->param[2], 0, 10);
		if (y < ctrl->params.height && x < ctrl->params.width &&
		    add_item(ctrl, cl, y, x))
			add_pending(cl, strdup("Command [SPAWN] successful"));
		else
			add_pending(cl, strdup(usage));
	} else
		add_pending(cl, strdup(usage));
	regfree(&preg);
}