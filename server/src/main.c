/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** main
*/

#include "server.h"
#include "socket/src/socket.h"
#include "list/src/list.h"
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
	CHECK(llist_push(control->clients, 1, client), == -1, false);
	return (true);
}

bool handle_request(control_t *control)
{
	(void)control;
	return (true);
}

bool control_init(control_t *control)
{
	CHECK(control->clients = llist_init(), == 0, false);
	return (true);
}

int main()
{
	control_t control = {0};

	CHECK(control_init(&control), == false, false);
	CHECK(control.fd = create_server(4242), == -1, 84);
	CHECK(poll_add(&control.list, control.fd, POLLIN), == 0, 84);
	while (1) {
		CHECK(poll_wait(control.list, -1), == -1, 84);
		if (poll_canread(control.list, control.fd)) {
			CHECK(add_new_client(&control), == false, 84);
		}
		else
			handle_request(&control);
	}
	return 0;
}
