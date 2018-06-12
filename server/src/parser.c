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

static void parse_cmd(client_t *cl)
{
	char *cmd = strdup(cl->cmd.cmd);
	char *tmp = strsep(&cmd, " ");

	memset(cl->cmd.name, 0, CMD_SIZE);
	memcpy(cl->cmd.name, tmp, strlen(tmp));
	do {
		tmp = strsep(&cmd, " ");
		if (tmp && tmp[0])
			add_param(&cl->cmd, tmp);
	} while (tmp);
}

static void extract_found_cmd(client_t *client, char *tmp, int csize)
{
	int len = client->cmd.rbuf.size;
	char *rbuf = client->cmd.rbuf.buffer;

	tmp[csize - 1] = 0;
	csize += 1;
	for (int i = 0; i < csize; ++i) {
		client->cmd.cmd[i % CMD_SIZE] = tmp[i];
		rbuf[(client->cmd.rbuf.start + i) % len] = 0;
	}
	client->cmd.rbuf.start = (client->cmd.rbuf.start + csize) % len;
	parse_cmd(client);
}

static bool retrieve_cmd(client_t *client)
{
	bool loop = false;
	int csize = 0;
	int epos = client->cmd.rbuf.end;
	int len = client->cmd.rbuf.size;
	int spos = client->cmd.rbuf.start;
	char *rbuf = client->cmd.rbuf.buffer;
	char tmp[RBUFFER_SIZE] = {0};

	for (; !loop && spos != epos; spos = (spos + 1) % len) {
		tmp[csize] = rbuf[spos];
		csize += 1;
		loop = (rbuf[spos] == '\n');
	}
	if (loop)
		extract_found_cmd(client, tmp, csize);
	return (loop);
}

size_t extract_cmd(control_t *ctrl, client_t *cl)
{
	bool loop;

	do {
		loop = retrieve_cmd(cl);
		if (loop)
			proceed_cmd(ctrl, cl);
	} while (loop);
	return (true);
}