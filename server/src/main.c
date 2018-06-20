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

bool add_new_client(control_t *ctrl)
{
	client_t *client;
	struct sockaddr_in addr = {0};
	socklen_t size = sizeof(addr);

	CHECK(client = calloc(1, sizeof(client_t)), == 0, false);
	CHECK(client->cmd = llist_init(), == 0, false);
	CHECK(client->pending = llist_init(), == 0, false);
	client->inventory[FOOD] = 10;
	client->rbuf.size = RBUFFER_SIZE;
	client->state = ANONYMOUS;
	client->pos.x = rand() % ctrl->params.width;
	client->pos.y = rand() % ctrl->params.height;
	CHECK(client->fd = accept(ctrl->fd, (struct sockaddr *)&addr, &size),
		== -1, false);
	CHECK(inet_ntop(AF_INET, client->ip, (void *)&addr, size), == 0,
		false);
	CHECK(client->node =
			poll_add(&ctrl->list, client->fd, POLLIN | POLLOUT),
		== 0, false);
	llist_push(client->pending, 1, strdup(WELCOME_MSG));
	CHECK(llist_push(ctrl->clients, 1, client), == -1, false);
	serialize_player(client);
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
	return (true);
}

bool append_special_client(control_t *control, client_t *client, char *type)
{
	if (lstr_equals(type, "gui"))
		client->state = GUI;
	else if (lstr_equals(type, "admin"))
		client->state = ADMIN;
	else {
		llist_push(client->pending, 1, strdup(KO_MSG));
		return (false);
	}
	llist_push(client->pending, 1, strdup(OK_MSG));
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
			str = lstr_concat(strdup(""), 3, LSTR_INT,
			                  (int)(control->teams[i].av));
			llist_push(client->pending, 1, str);
			str = lstr_concat(strdup(""), 3, LSTR_INT,
			                  (int)(control->params.width), LSTR_CHAR, ' ',
			                  LSTR_INT, (int)(control->params.height));
			llist_push(client->pending, 1, str);
			client->state = PLAYER;
			printf("Triggered\n");
			return (true);
		}

	llist_push(client->pending, 1, strdup(KO_MSG));
	return (true);
}

bool proceed_clients(control_t *ctrl)
{
	bool to_evict;
	client_t *cl;

	for (size_t i = 0; i < ctrl->clients->length; ++i) {
		cl = llist_at(ctrl->clients, (size_t) i);
		to_evict = ((cl->node->revt & POLLHUP) == POLLHUP);
		if (cl->cmd->length && cl->state == ANONYMOUS)
			append_to_team(ctrl, cl);
		else if (cl->cmd->length && cl->state == PLAYER && !to_evict &&
			cl->task.type == NONE)
			proceed_cmd(ctrl, cl);
		if (cl->task.type != NONE && !to_evict)
			exec_task(ctrl, cl);
		if (!to_evict && cl->pending->length &&
			(cl->node->revt & POLLOUT))
			write_to_client(ctrl, cl);
		if (to_evict)
			evict_client(ctrl, cl);
	}
	return (true);
}

bool handle_client(control_t *control, client_t *cl, size_t idx)
{
	bool to_evict = ((cl->node->revt & POLLHUP) == POLLHUP);

	(void)(idx);
	if (!to_evict && (cl->node->revt & POLLIN))
		to_evict = !(receive_data(cl) && extract_rbuf_cmd(cl));
	return (to_evict == false);
}

bool consume_food(control_t *control, client_t *client)
{
	(void)(control);
	client->food_delay -= 1;
	if (client->food_delay == 0) {
		client->food -= 1;
		client->food_delay = FOOD_DELAY;
	}
	if (client->food == 0)
		; // TODO: Kill player
	return (true);
}

bool handle_request(control_t *control)
{
	list_t *tmp = control->clients;
	client_t *cl;

	control->clients = llist_filter(control->clients,
		(bool (*)(void *, void *, size_t))(handle_client), control);
	llist_destroy(tmp);
	// dprintf(1, "Client count: %lu\n", llist_size(control->clients));
	for (list_elem_t *it = control->clients->head; it; it = it->next) {
		cl = it->payload;
		cl->node->evt = POLLIN | (cl->pending->length ? POLLOUT : 0);
		consume_food(control, cl);
	}
	return (true);
}

bool ctrl_init(control_t *ctrl)
{
	CHECK(ctrl->clients = llist_init(), == 0, false);
	CHECK(init_map(ctrl), == false, false);
	CHECK(place_resources(ctrl), == false, false);
	CHECK(team_init(ctrl), == false, false);
	return (true);
}

static bool perfom_poll_actions(control_t *ctrl)
{
	if (poll_canread(ctrl->list, ctrl->fd)) {
		CHECK(add_new_client(ctrl), == false, 84);
	}
	else if (ctrl->clients->length)
		handle_request(ctrl);
	return (true);
}

static bool cycle_adjustment(control_t *ctrl)
{
	struct timeval stop, start;
	long tr = (long)(round(1 * 1e3 / ctrl->params.tickrate));
	long ms = tr;

	CHECK(gettimeofday(&start, NULL), == -1, -1);
	while (ms > 0) {
		if (poll_wait(ctrl->list, (int)ms) > 0)
			perfom_poll_actions(ctrl);
		CHECK(gettimeofday(&stop, NULL), == -1, -1);
		ms = tr - (long)((round((stop.tv_sec * 1e6 + stop.tv_usec)
			- (start.tv_sec * 1e6 + start.tv_usec)) / 1e3));
	}
	return (true);
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
	while (1) {
		CHECK(ret = cycle_adjustment(&ctrl), == false, 84);
		proceed_clients(&ctrl);
	}
	return (0);
}
