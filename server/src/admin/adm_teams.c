/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include <regex.h>
#include "server.h"

void adm_teams(control_t *ctrl, client_t *cl)
{
	regex_t preg;
	cmd_t *cmd = cl->cmd->head->payload;
	char pattern[] = R"(?:team (?:add|del) [[:alnum:]]*$)";
	char usage[] = "TEAMS: [add|del] [team_name]";

	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE);
	if (cmd->nparam == 2 && !regexec(&preg, cmd->cmd, 0, NULL, 0)) {
		if (!strcmp(cmd->param[0], "add"))
			team_add(ctrl, cmd->param[1]);
		else
			team_remove(ctrl, cmd->param[1]);
		add_pending(cl, "Command [TEAMS] successful");
	}
	else
		add_pending(cl, strdup(usage));
	regfree(&preg);
}