/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** main
*/

#include "list/src/list.h"
#include "server.h"
#include "socket/src/socket.h"
#include "str/src/str.h"
#include <arpa/inet.h>
#include <math.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void add_pending(client_t *cl, char *msg)
{
	llist_push(cl->pending, 1, msg);
	cl->node->evt = POLLIN | POLLOUT;
}

bool add_new_client(control_t *ctrl)
{
	client_t *client;
	struct sockaddr_in addr = {0};
	socklen_t size = sizeof(addr);
	static size_t ids = 0;

	CHECK(client = calloc(1, sizeof(client_t)), == 0, false);
	CHECK(client->cmd = llist_init(), == 0, false);
	CHECK(client->pending = llist_init(), == 0, false);
	client->id = ids++;
	client->level = 1;
	client->inventory[FOOD] = 10;
	client->food_delay = FOOD_DELAY;
	client->rbuf.size = RBUFFER_SIZE;
	client->state = ANONYMOUS;
	client->pos.x = rand() % ctrl->params.width;
	client->pos.y = rand() % ctrl->params.height;
	dprintf(2, "spawn position: (%lu,%lu)\n", client->pos.x,
		client->pos.y);
	CHECK(client->fd = accept(ctrl->fd, (struct sockaddr *)&addr, &size),
		== -1, false);
	CHECK(inet_ntop(AF_INET, client->ip, (void *)&addr, size), == 0,
		false);
	CHECK(client->node =
			poll_add(&ctrl->list, client->fd, POLLIN | POLLOUT),
		== 0, false);
	add_pending(client, strdup(WELCOME_MSG));
	CHECK(llist_push(ctrl->clients, 1, client), == -1, false);
	return (true);
}

ssize_t receive_data(client_t *cl)
{
	ssize_t ret = 0;
	char data[CMD_SIZE];
	int len = cl->rbuf.size;
	int *epos = &cl->rbuf.end;
	char *rbuf = cl->rbuf.buffer;

	ret = recv(cl->fd, data, CMD_SIZE, 0);
	for (int i = 0; i < ret; ++i) {
		rbuf[*epos] = data[i];
		*epos = (*epos + 1) % len;
	}
	return (ret);
}

bool evict_client(control_t *control, client_t *cl)
{
	event_player_death(control, cl, "disconnection");
	team_remove_client(control, cl);
	poll_rm(&control->list, cl->fd);
	llist_clear(cl->pending, true);
	llist_destroy(cl->pending);
	close(cl->fd);
	free(cl);
	return (true);
}

bool write_to_client(control_t *control, client_t *cl)
{
	char *str = llist_remove(cl->pending, 0);

	(void)(control);
	if (str)
		dprintf(cl->fd, "%s\n", str);
	free(str);
	if (cl->pending->length == 0)
		cl->node->evt = POLLIN;
	return (true);
}

bool append_special_client(control_t *control, client_t *client, char *type)
{
	(void)(control);
	if (lstr_equals(type, "gui"))
		client->state = GUI;
	else if (lstr_equals(type, "admin"))
		client->state = ADMIN;
	else {
		add_pending(client, strdup(KO_MSG));
		return (false);
	}
	add_pending(client, strdup(OK_MSG));
	return (true);
}

bool append_to_team(control_t *control, client_t *client)
{
	cmd_t *cmd = llist_remove(client->cmd, 0);
	char *str;

	if (lstr_equals(cmd->name, "gui") || lstr_equals(cmd->name, "admin"))
		return (append_special_client(control, client, cmd->name));
	for (size_t i = 0; i < control->params.nteam; ++i)
		if (control->teams[i].av &&
			lstr_equals(control->teams[i].name, cmd->name)) {
			team_add_client(control, client, cmd->name);
			str = lstr_concat(strdup(""), 1, LSTR_INT,
				(int)(control->teams[i].av));
			add_pending(client, str);
			str = lstr_concat(strdup(""), 3, LSTR_INT,
				(int)(control->params.width), LSTR_CHAR, ' ',
				LSTR_INT, (int)(control->params.height));
			add_pending(client, str);
			client->state = PLAYER;
			event_player_join(control, client);
			return (true);
		}
	add_pending(client, strdup(KO_MSG));
	return (true);
}

bool proceed_clients(control_t *ctrl)
{
	bool to_evict;
	client_t *cl;

	for (size_t i = 0; i < ctrl->clients->length; ++i) {
		cl = llist_at(ctrl->clients, (size_t)i);
		to_evict = ((cl->node->revt & POLLHUP) == POLLHUP);
		if (cl->cmd->length && cl->state == ANONYMOUS)
			append_to_team(ctrl, cl);
		else if (cl->cmd->length && cl->state != ANONYMOUS &&
			!to_evict && cl->task.type == NONE)
			proceed_cmd(ctrl, cl);
		if (cl->task.type != NONE && !to_evict)
			exec_task(ctrl, cl);
		if (to_evict)
			evict_client(ctrl, cl);
	}
	return (to_evict == false);
}

bool handle_client(control_t *control, client_t *cl, size_t idx)
{
	bool to_evict = ((cl->node->revt & POLLHUP) == POLLHUP);

	(void)(control);
	(void)(idx);
	if (!to_evict && (cl->node->revt & POLLIN))
		to_evict = !(receive_data(cl) && extract_rbuf_cmd(cl));
	return (to_evict == false);
}

void free_player(control_t *control, client_t *client)
{
	event_player_death(control, client, "starvation");
	dprintf(client->fd, "dead\n");
	close(client->fd);
	poll_rm(&control->list, client->fd);
	llist_clear(client->pending, true);
	llist_destroy(client->pending);
	team_remove_client(control, client);
	free(client);
}

bool consume_food(control_t *control)
{
	ssize_t size = ((ssize_t)(control->clients->length)) - 1;
	client_t *client;

	for (ssize_t i = size; i >= 0; i -= 1) {
		client = llist_at(control->clients, i);
		if (client->state != PLAYER)
			continue;
		client->food_delay -= (client->food_delay != 0);
		if (client->food_delay == 0) {
			client->inventory[FOOD] -=
				(client->inventory[FOOD] != 0);
			client->food_delay = FOOD_DELAY;
		}
		if (client->inventory[FOOD] == 0) {
			free_player(control, client);
			llist_remove(control->clients, i);
		}
	}
	return (true);
}

bool find_evicted(void *t, void *s, size_t l)
{
	(void)l;
	return (s == t);
}

bool handle_request(control_t *control)
{
	list_t *tmp = control->clients;
	client_t *cl;

	control->clients = llist_filter(control->clients,
		(bool (*)(void *, void *, size_t))(handle_client), control);
	for (size_t i = 0; i < tmp->length; ++i) {
		cl = llist_at(tmp, i);
		if (!llist_find(control->clients, find_evicted, cl))
			evict_client(control, cl);
	}
	for (list_elem_t *it = control->clients->head; it; it = it->next) {
		cl = it->payload;
		cl->node->evt = POLLIN | (cl->pending->length ? POLLOUT : 0);
	}
	return (true);
}

bool ctrl_init(control_t *ctrl)
{
	CHECK(ctrl->clients = llist_init(), == 0, false);
	CHECK(ctrl->eggs = llist_init(), == 0, false);
	CHECK(init_map(ctrl), == false, false);
	CHECK(place_resources(ctrl), == false, false);
	CHECK(team_init(ctrl), == false, false);
	return (true);
}

static bool perfom_poll_actions(control_t *ctrl)
{
	client_t *client;

	if (poll_canread(ctrl->list, ctrl->fd)) {
		CHECK(add_new_client(ctrl), == false, 84);
	}
	else if (ctrl->clients->length)
		handle_request(ctrl);
	for (list_elem_t *it = ctrl->clients->head; it; it = it->next) {
		client = it->payload;
		if (client->pending->length && (client->node->revt & POLLOUT))
			write_to_client(ctrl, client);
	}
	return (true);
}

static bool cycle_adjustment(control_t *ctrl)
{
	struct timeval stop, start;
	long tr = (long)(round(1 * 1e3 / ctrl->params.tickrate));
	long ms = tr;
	double i1;
	double i2;

	CHECK(gettimeofday(&start, NULL), == -1, -1);
	while (ms > 0) {
		if (poll_wait(ctrl->list, (int)ms) > 0)
			perfom_poll_actions(ctrl);
		CHECK(gettimeofday(&stop, NULL), == -1, -1);
		i1 = stop.tv_sec * 1e6 + stop.tv_usec;
		i2 = start.tv_sec * 1e6 + start.tv_usec;
		ms = tr - (long)((round(i1 - i2) / 1e3));
	}
	return (true);
}

void consume_eggs(control_t *control)
{
	ssize_t size = ((ssize_t)(control->eggs->length)) - 1;
	egg_t *egg;

	for (ssize_t i = size; i >= 0; i -= 1) {
		egg = llist_at(control->eggs, i);
		egg->delay -= 1;
		if (egg->delay == 0) {
			egg->team->cl =
				realloc(egg->team->cl, ++egg->team->size);
			egg->team->av += 1;
			event_egg_hatch(control, egg);
			llist_remove(control->eggs, i);
		}
	}
}

int main(int ac, const char **av)
{
	control_t ctrl = {0};
	params_t params = {false, 4242, 20, 20, 0, 0, 5, 100};
	int ret;

	srand(getpid() * time(0));
	ctrl.params = params;
	if (parse_args((size_t)(ac), av, &ctrl.params) == false)
		return (84);
	if (ctrl.params.help)
		return (disp_help(av[0]));
	CHECK(ctrl_init(&ctrl), == false, 84);
	CHECK(ctrl.fd = create_server(ctrl.params.port), == -1, 84);
	CHECK(poll_add(&ctrl.list, ctrl.fd, POLLIN), == 0, 84);
	display_map(&ctrl);
	while (1) {
		CHECK(ret = cycle_adjustment(&ctrl), == false, 84);
		proceed_clients(&ctrl);
		// consume_food(&ctrl);
		consume_eggs(&ctrl);
	}
	return (0);
}
