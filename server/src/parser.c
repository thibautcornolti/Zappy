/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

static void add_param(cmd_t *cmd, char *str)
{
	cmd->param = realloc(cmd->param, sizeof(char *) * (cmd->nparam + 1));
	if (cmd->param) {
		cmd->param[cmd->nparam] = strdup(str);
		cmd->nparam += 1;
	}
}

static void parse_cmd(cmd_t *item)
{
	char *cmd = strdup(item->cmd);
	char *save = cmd;
	char *tmp = strsep(&cmd, " ");

	memset(item->name, 0, CMD_SIZE);
	memcpy(item->name, tmp, strlen(tmp));
	do {
		tmp = strsep(&cmd, " ");
		if (tmp && tmp[0])
			add_param(item, tmp);
	} while (tmp);
	free(save);
}

static void extract_found_cmd(client_t *client, char *tmp, int csize)
{
	int len = client->rbuf.size;
	cmd_t *cmd = calloc(sizeof(cmd_t), 1);
	char *rbuf = client->rbuf.buffer;

	if (!cmd)
		return;
	tmp[csize - 1] = 0;
	for (int i = 0; i < csize; ++i) {
		cmd->cmd[i % CMD_SIZE] = tmp[i];
		rbuf[(client->rbuf.start + i) % len] = 0;
	}
	client->rbuf.start = (client->rbuf.start + csize) % len;
	parse_cmd(cmd);
	llist_push(client->cmd, 1, cmd);
}

static bool retrieve_cmd(client_t *client)
{
	bool loop = false;
	int csize = 0;
	int epos = client->rbuf.end;
	int len = client->rbuf.size;
	int spos = client->rbuf.start;
	char *rbuf = client->rbuf.buffer;
	char tmp[RBUFFER_SIZE] = {0};

	for (; !loop && spos != epos; spos = (spos + 1) % len) {
		tmp[csize] = rbuf[spos];
		csize += 1;
		loop = (rbuf[spos] == '\n');
	}
	if (loop && client->cmd && llist_size(client->cmd) < 10)
		extract_found_cmd(client, tmp, csize);
	return (loop);
}

size_t extract_rbuf_cmd(client_t *cl)
{
	bool loop = true;

	for (; loop; loop = retrieve_cmd(cl));
	return (true);
}