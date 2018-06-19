/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include <regex.h>
#include "server.h"

void lvlup_player(size_t id)
{
	(void)id;
}

void adm_lvlup(control_t *ctrl, client_t *cl)
{
	(void)ctrl;
	regex_t pregit g;
	cmd_t *cmd = cl->cmd->head->payload;
	char pattern[] = R"(?:\/lvlup \d+$)";
	char usage[] = "LVLUP: [id]";

	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE);
	if (cmd->nparam == 1 && !regexec(&preg, cmd->cmd, 0, NULL, 0)) {
		lvlup_player(strtoul(cmd->param[0], 0, 10));
		llist_push(cl->pending, 1, "Command [SPAWN] successful");
	}
	else
		llist_push(cl->pending, 1, strdup(usage));
	regfree(&preg);
}