/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** main
*/

#include "list/src/list.h"
#include "server.h"
#include "socket/src/socket.h"
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
	client->rbuf.size = RBUFFER_SIZE;
	client->fd = accept(ctrl->fd, (struct sockaddr *)&addr, &size);
	CHECK(inet_ntop(AF_INET, client->ip, (void *)&addr, size), == 0,
		false);
	CHECK(client->node = poll_add(&ctrl->list, client->fd, POLLIN), == 0,
		false);
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

bool handle_client(control_t *ctrl, client_t *cl, size_t idx)
{
	bool to_evict = ((cl->node->revt & POLLHUP) == POLLHUP);
	char *str;

	(void)(idx);
	if (!to_evict && (cl->node->revt & POLLIN)) {
		to_evict = !(receive_data(cl) && extract_rbuf_cmd(cl));
		if (!to_evict && cl->task.type == NONE)
			proceed_cmd(ctrl, cl);
	}
	if (!to_evict && cl->pending->length && (cl->node->revt & POLLOUT)) {
		str = llist_remove(cl->pending, 0);
		if (str)
			write(cl->fd, str, strlen(str));
	}
	if (to_evict) {
		poll_rm(&ctrl->list, cl->fd);
		llist_clear(cl->pending, true);
		llist_destroy(cl->pending);
		close(cl->fd);
		free(cl);
	}
	return (true);
}

bool handle_request(control_t *ctrl)
{
	team_t *team;
	size_t client_count;

	ctrl->clients = llist_filter(ctrl->clients,
		(bool (*)(void *, void *, size_t))handle_client, ctrl);
	for (size_t i = 0; i < ctrl->params.nteam; ++i) {
		team = &(ctrl->teams[i]);
		client_count = ctrl->params.nclt - team->av;
		for (size_t j = 0; j < client_count; ++j)
			CHECK(exec_task(ctrl, team->cl[j]), == false, false);
	}
	return (true);
}

bool ctrl_init(control_t *ctrl)
{
	CHECK(ctrl->clients = llist_init(), == 0, false);
	CHECK(init_map(ctrl), == false, false);
	CHECK(place_resources(ctrl), == false, false);
	return (true);
}

static int cycle_adjustment(control_t *ctrl, bool await)
{
	static long ms = 0;
	struct timeval stop;
	static struct timeval start;
	long tr = (long)round(1 * 1e3 / ctrl->params.tickrate);

	if (!await) {
		ms = (tr > ms) ? tr : 2 * tr - ms;
		gettimeofday(&start, NULL);
	}
	else {
		gettimeofday(&stop, NULL);
		ms = (long)((round(stop.tv_usec - start.tv_usec) / 1e3));
		ms = (long)((ms < 0) ? 1e3 + ms : ms);
		usleep((__useconds_t)((ms < tr) ? tr - ms : 0));
	}
	return ((int)(ms));
}

int main(int ac, const char **av)
{
	control_t ctrl = {0};
	params_t params = {false, 4242, 20, 20, 0, 0, 5, 100};
	int ret;

	ctrl.params = params;
	CHECK(parse_args((size_t)(ac), av, &ctrl.params), == false, 84);
	if (ctrl.params.help)
		srand(getpid() * time(0));
	ctrl.params = params;
	CHECK(parse_args((size_t)(ac), av, &ctrl.params), == false, 84);
	if (ctrl.params.help)
		return (disp_help(av[0]));
	CHECK(ctrl_init(&ctrl), == false, 84);
	CHECK(ctrl.fd = create_server(ctrl.params.port), == -1, 84);
	CHECK(poll_add(&ctrl.list, ctrl.fd, POLLIN), == 0, 84);

	while (1) {
		CHECK(ret = poll_wait(
			      ctrl.list, cycle_adjustment(&ctrl, false)),
			== -1, 84);
		if (poll_canread(ctrl.list, ctrl.fd)) {
			CHECK(add_new_client(&ctrl), == false, 84);
		}
		else
			handle_request(&ctrl);
		cycle_adjustment(&ctrl, true);
	}
	return (0);
}