/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

static tuple_t player[] = {{"forward", &cmd_forward}, {"right", &cmd_right},
	{"left", &cmd_left}, {"look", &cmd_look},
	{"inventory", &cmd_inventory}, {"broadcast", &cmd_broadcast},
	{"connect_nbr", &cmd_connect_nbr}, {"fork", &cmd_fork},
	{"eject", &cmd_eject}, {"take", &cmd_take}, {"set", &cmd_set},
	{"incantation", &cmd_incantation}};

static tuple_t gui[] = {{"map-size", &gui_map_size},
	{"entities", &gui_entities}, {"tile", &gui_tile},
	{"player", &gui_player}, {"tickrate", &gui_tickrate}};

static tuple_t admin[] = {{"getids", &adm_getids}, {"cast", &adm_cast},
	{"killall", &adm_killall}, {"lvlup", &adm_lvlup}, {"move", &adm_move},
	{"spawn", &adm_spawn}, {"teams", &adm_teams},
	{"tickrate", &adm_tickrate}};

static tuple_t *commands[] = {[PLAYER] = player, [GUI] = gui, [ADMIN] = admin};

static int cmd_amount[] = {[PLAYER] = PLAYER_CMD_COUNT,
	[GUI] = GUI_CMD_COUNT,
	[ADMIN] = ADMIN_CMD_COUNT};

static void cmd_unknown(client_t *cl, cmd_t *cmd)
{
	dprintf(2, "[%s] Unknown command %s\n", cl->ip, cmd->name);
	if (cl->state == GUI)
		finalize_json(cl, emit_command_error("Unknown command."), cmd);
	else
		add_pending(cl, strdup(KO_MSG));
}

void clear_cmd(cmd_t *cmd)
{
	if (cmd && cmd->param) {
		for (int i = 0; i < cmd->nparam && cmd->param[i]; ++i)
			free(cmd->param[i]);
		free(cmd->param);
		free(cmd);
	}
}

void show_cmd(cmd_t *cmd)
{
	printf("CMD: %s\n", cmd->name);
	for (int i = 0; i < cmd->nparam; ++i)
		printf("[%d] %s\n", i, cmd->param[i]);
}

void proceed_cmd(control_t *ctrl, client_t *cl)
{
	cmd_t *cmd = cl->cmd->head->payload;

	(void)(ctrl);
	show_cmd(cmd);
	if (cmd->name[0] == 0 && cl->state == GUI) {
		finalize_json(cl,
			emit_syntax_error("Non-JSON payload received."), cmd);
		clear_cmd(llist_remove(cl->cmd, 0));
		return;
	}
	for (int i = 0; i < cmd_amount[cl->state]; ++i)
		if (!strcasecmp(cmd->name, commands[cl->state][i].cmd)) {
			commands[cl->state][i].func(ctrl, cl);
			break;
		}
		else if (i + 1 == cmd_amount[cl->state])
			cmd_unknown(cl, cmd);
	clear_cmd(llist_remove(cl->cmd, 0));
}
