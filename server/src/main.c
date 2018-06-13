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
#include <sys/socket.h>
#include <sys/types.h>

bool add_new_client(control_t *control)
{
	client_t *client;
	struct sockaddr_in addr = {0};
	socklen_t size = sizeof(addr);

	CHECK(client = calloc(sizeof(client_t), 1), == 0, false);
	CHECK(client->cmd = llist_init(), == 0, false);
	CHECK(client->pending = llist_init(), == 0, false);
	client->rbuf.size = RBUFFER_SIZE;
	client->fd = accept(control->fd, (struct sockaddr *)&addr, &size);
	CHECK(inet_ntop(AF_INET, client->ip, (void *)&addr, size), == 0,
		false);
	CHECK(client->node = poll_add(&control->list, client->fd, POLLIN),
		== 0, false);
	CHECK(llist_push(control->clients, 1, client), == -1, false);
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

bool handle_client(control_t *control, client_t *cl, size_t idx)
{
	bool to_evict = ((cl->node->revt & POLLHUP) == POLLHUP);
	char *str;

	(void)(idx);
	if (!to_evict && (cl->node->revt & POLLIN)) {
		to_evict = !(receive_data(cl) && extract_rbuf_cmd(cl));
		if (!to_evict)
			proceed_cmd(control, cl);
	}
	if (!to_evict && cl->pending->length && (cl->node->revt & POLLOUT)) {
		str = llist_remove(cl->pending, 0);
		if (str)
			write(cl->fd, str, strlen(str));
	}
	if (to_evict) {
		poll_rm(&control->list, cl->fd);
		llist_clear(cl->pending, true);
		llist_destroy(cl->pending);
		close(cl->fd);
		free(cl);
	}
	return (true);
}

bool handle_request(control_t *control)
{
	control->clients = llist_filter(control->clients,
		(bool (*)(void *, void *, size_t))handle_client, control);
	return (true);
}

bool control_init(control_t *control)
{
	CHECK(control->clients = llist_init(), == 0, false);
	return (true);
}

int main(int ac, const char **av)
{
	control_t control = {0};
	params_t params = {false, 4242, 20, 20, 0, 0, 5, 100};
	int ret;

	control.params = params;
	CHECK(parse_args((size_t)(ac), av, &control.params), == false, 84);
	if (control.params.help)
		return (disp_help(av[0]));
	CHECK(control_init(&control), == false, 84);
	CHECK(control.fd = create_server(control.params.port), == -1, 84);
	CHECK(poll_add(&control.list, control.fd, POLLIN), == 0, 84);
	while (1) {
		CHECK(ret = poll_wait(control.list, -1), == -1, 84);
		if (poll_canread(control.list, control.fd)) {
			CHECK(add_new_client(&control), == false, 84);
		}
		else
			handle_request(&control);
	}
	return (0);
}