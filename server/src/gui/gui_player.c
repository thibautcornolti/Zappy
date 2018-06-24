/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by brinc
*/

#include "server.h"

static bool str_isnum(const char *str)
{
	for (size_t i = 0; str[i]; ++i)
		if (isdigit(str[i]) == 0)
			return (false);
	return (str[0] != 0);
}

static bool convert_to_number(size_t *ret, elem_t *elem)
{
	if (lstr_equals(elem->type, "number"))
		*ret = *((double *)(&elem->data));
	else if (lstr_equals(elem->type, "string") && str_isnum(elem->data))
		*ret = strtoul(elem->data, 0, 10);
	else
		return (false);
	return (true);
}

bool find_client(size_t *id, client_t *client, size_t idx)
{
	(void)(idx);
	return (*id == client->id);
}

static void send_player_info(client_t *client, client_t *cl, cmd_t *cmd)
{
	object_t *ret = lobj_init();

	lobj_set(ret, "type", "string", strdup("response"));
	lobj_set(ret, "response-type", "string", strdup("player"));
	lobj_set(ret, "data", "object", serialize_player(cl));
	finalize_json(client, ret, cmd);
}

void gui_player(control_t *control, client_t *client)
{
	size_t i;
	client_t *cl;
	cmd_t *cmd = client->cmd->head->payload;
	elem_t *elem = lobj_get(cmd->json->root->data, "id");

	if (elem == 0) {
		finalize_json(
			client, emit_command_error("Bad ID parameter."), cmd);
		return;
	}
	convert_to_number(&i, elem);
	cl = llist_find(control->clients,
		(bool (*)(void *, void *, size_t))(find_client), &i);
	if (cl == 0)
		finalize_json(
			client, emit_command_error("Player not found."), cmd);
	else
		send_player_info(client, cl, cmd);
}
