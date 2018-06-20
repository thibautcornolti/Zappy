/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by brinc
*/

#include <regex.h>
#include "server.h"

static char *add_client_id(client_t *cl, char *str)
{
	if (str[strlen(str) - 1] != ' ')
		str = lstr_concat(str, 1, LSTR_STR, ", ");
	str = lstr_concat(str, 1, LSTR_INT, (int) cl->id);
	return (str);
}

void adm_getids(control_t *ctrl, client_t *cl)
{
	regex_t preg;
	cmd_t *cmd = cl->cmd->head->payload;
	char pattern[] = R"(getids$)";
	char usage[] = "getids";
	char *str;
	client_t *client;

	printf("[%s]\n", cmd->cmd);
	regcomp(&preg, pattern, REG_NOSUB | REG_ICASE);
	if (!cmd->nparam && !regexec(&preg, cmd->cmd, 0, NULL, 0)) {
		str = lstr_concat(strdup(""), 1, LSTR_STR, "ids: ");
		for (size_t i = 0; i < ctrl->clients->length; ++i) {
			client = llist_at(ctrl->clients, i);
			if (client->state == PLAYER)
				str = add_client_id(client, str);
		}
		add_pending(cl, str);
	} else
		add_pending(cl, strdup(usage));
	regfree(&preg);
}