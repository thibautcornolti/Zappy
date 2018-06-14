/*
** EPITECH PROJECT, 2018
** PSU_zappy_2017
** File description:
** server.h
*/

#pragma once

#include "list/src/list.h"
#include "poll/src/common.h"
#include <netinet/in.h>
#include <stddef.h>

#define RBUFFER_SIZE 4096
#define CMD_SIZE 2048
#define CMD_COUNT 12

#define OK_MSG "ok"
#define KO_MSG "ko"
#define HELP_MSG                                                              \
	"USAGE: %s -p port -x width -y height -n name1 name2 ... -c "         \
	"clientsNb -f freq\n"                                                 \
	"\tport\t\tis the port number\n"                                      \
	"\twidth\t\tis the width of the world\n"                              \
	"\theight\t\tis the height of the world\n"                            \
	"\tnameX\t\tis the name of the team X\n"                              \
	"\tclientsNb\tis the number of authorized clients per team\n"         \
	"\tfreq\t\tis the reciprocal of time unit for execution of actions\n"

#ifndef CHECK
#define CHECK(x, y, z)                                                        \
	if ((x)y) {                                                           \
		dprintf(2, "[%s:%d]: %s\n", __FILE__, __LINE__,               \
			__FUNCTION__);                                        \
		return (z);                                                   \
	}
#endif

typedef enum facing_e { NORTH = 0, EAST, SOUTH, WEST } facing_t;

typedef enum state_e { ANONYMOUS, PLAYING } state_t;

typedef enum task_type_e {
	NONE = 0,
	FORWARD,
	LEFT,
	RIGHT,
	LOOK,
	INVENTORY,
	BROADCAST,
	CONNECT_NBR,
	FORK,
	EJECT,
	TAKE,
	SET,
	INCANTATION
} task_type_t;

typedef enum item_s {
	FOOD,
	LINEMATE,
	DERAUMERE,
	SIBUR,
	MENDIANE,
	PHIRAS,
	THYSTAME
} item_t;

typedef struct cell_s {
	size_t x;
	size_t y;
	list_t *items;
} cell_t;

typedef struct rbuf_s {
	char buffer[RBUFFER_SIZE];
	int start;
	int end;
	int size;
} rbuf_t;

typedef struct cmd_s {
	char cmd[CMD_SIZE];
	char name[CMD_SIZE];
	char **param;
	int nparam;
} cmd_t;

typedef struct params_s {
	bool help;
	size_t port;
	size_t width;
	size_t height;
	char **teams;
	size_t nteam;
	size_t nclt;
	size_t tickrate;
} params_t;

typedef struct vec2_s {
	size_t x;
	size_t y;
} vec2_t;

typedef struct task_s {
	size_t time;
	task_type_t type;
} task_t;

typedef struct client_s {
	int fd;
	char ip[INET_ADDRSTRLEN];
	list_t *cmd;
	rbuf_t rbuf;
	size_t id;
	vec2_t pos;
	facing_t facing;
	size_t food;
	poll_t *node;
	list_t *pending;
	task_t task;
	struct team_s *team;
} client_t;

typedef struct team_s {
	char *name;
	client_t **cl;
	size_t av;
} team_t;

typedef struct control_s {
	int fd;
	poll_t *list;
	list_t *clients;
	team_t *teams;
	cell_t *map;
	params_t params;
} control_t;

typedef struct tuple_s {
	char *cmd;
	void (*func)(control_t *, client_t *);
} tuple_t;

typedef struct param_parse_s {
	char *cmd;
	bool (*func)(size_t, const char **, params_t *, size_t *);
} param_parse_t;

void cmd_forward(control_t *, client_t *);
void cmd_right(control_t *, client_t *);
void cmd_left(control_t *, client_t *);
void cmd_look(control_t *, client_t *);
void cmd_inventory(control_t *, client_t *);
void cmd_broadcast(control_t *, client_t *);
void cmd_connect_nbr(control_t *, client_t *);
void cmd_fork(control_t *, client_t *);
void cmd_eject(control_t *, client_t *);
void cmd_take(control_t *, client_t *);
void cmd_set(control_t *, client_t *);
void cmd_incantation(control_t *, client_t *);

void exec_forward(control_t *, client_t *);
void exec_right(control_t *, client_t *);
void exec_left(control_t *, client_t *);
void exec_look(control_t *, client_t *);
void exec_inventory(control_t *, client_t *);
void exec_broadcast(control_t *, client_t *);
void exec_connect_nbr(control_t *, client_t *);
void exec_fork(control_t *, client_t *);
void exec_eject(control_t *, client_t *);
void exec_take(control_t *, client_t *);
void exec_set(control_t *, client_t *);
void exec_incantation(control_t *, client_t *);

bool parse_port(size_t, const char **, params_t *, size_t *);
bool parse_width(size_t, const char **, params_t *, size_t *);
bool parse_height(size_t, const char **, params_t *, size_t *);
bool parse_names(size_t, const char **, params_t *, size_t *);
bool parse_clients(size_t, const char **, params_t *, size_t *);
bool parse_freq(size_t, const char **, params_t *, size_t *);
bool parse_help(size_t, const char **, params_t *, size_t *);

size_t extract_rbuf_cmd(client_t *);
void proceed_cmd(control_t *, client_t *);
bool exec_task(control_t *, client_t *);
bool place_resources(control_t *);

/*
** Teams
*/
bool team_add_client(control_t *, client_t *, char *);
bool team_remove_client(control_t *, client_t *);
void team_add(control_t *, char *);
bool team_remove(control_t *, char *);
void team_init(control_t *);
void team_realloc_arr(control_t *);
void team_release(team_t *);

/*
** Args parsing
*/
int disp_help(const char *);
bool parse_args(size_t , const char **, params_t *);

/*
** Map
*/
bool init_map(control_t *);
list_t *map_get(control_t *, size_t, size_t);
void map_set(control_t *, size_t, size_t, item_t);
void map_remove(control_t *, size_t, size_t, item_t);

/*
 * resource_gen
 */
bool place_resources(control_t *);