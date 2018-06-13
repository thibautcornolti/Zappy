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

	CHECK(client = calloc(sizeof(client_t), 0), == 0, false);
	client->fd = accept(control->fd, (struct sockaddr *)&addr, &size);
	CHECK(inet_ntop(AF_INET, client->ip, (void *)&addr, size), == 0,
		false);
	CHECK(client->node = poll_add(control->list, client->fd, POLLIN), == 0,
		false);
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

	if (!to_evict && (cl->node->revt & POLLIN)) {
		to_evict = !(receive_data(cl) && extract_rbuf_cmd(cl));
		if (!to_evict)
			proceed_cmd(control, cl);
	}
	if (!to_evict && cl->pending->length && (cl->node->revt & POLLOUT))
		; // TODO: Send to client
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
}

bool control_init(control_t *control)
{
	CHECK(control->clients = llist_init(), == 0, false);
	return (true);
}

int main()
{
	control_t control = {0};
	client_t cl = {0};
	char str[] = "test\n\nerfioerjriegjreo\nzef fez zef \n";

	cl.cmd = llist_init();
	 memcpy(cl.rbuf.buffer, str, strlen(str));
	 cl.rbuf.end = (int)strlen(str);
	 cl.rbuf.size = RBUFFER_SIZE;
	extract_rbuf_cmd(&cl);
	while (cl.cmd->length)
		proceed_cmd(&control, &cl);

//	CHECK(control_init(&control), == false, false);
//	CHECK(control.fd = create_server(4242), == -1, 84);
//	CHECK(poll_add(&control.list, control.fd, POLLIN), == 0, 84);
//	while (1) {
//		CHECK(poll_wait(control.list, -1), == -1, 84);
//		if (poll_canread(control.list, control.fd)) {
//			CHECK(add_new_client(&control), == false, 84);
//		}
//		else
//			handle_request(&control);
//	}
	return (0);
}
