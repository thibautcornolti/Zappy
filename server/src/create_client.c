/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** create_client
*/

#include "server.h"
#include <arpa/inet.h>

static size_t get_id(void)
{
	static size_t ids = 0;

	return (ids++);
}

static bool fill_ip(control_t *ctrl, client_t *client)
{
	struct sockaddr_in addr = {0};
	socklen_t size = sizeof(addr);

	CHECK(client->fd = accept(ctrl->fd, (struct sockaddr *)&addr, &size),
		== -1, false);
	CHECK(inet_ntop(AF_INET, client->ip, (void *)&addr, size), == 0,
		false);
	return (true);
}

static bool fill_base_stats(control_t *ctrl, client_t *client)
{
	client->level = 1;
	client->inventory[FOOD] = 10;
	client->food_delay = FOOD_DELAY;
	client->rbuf.size = RBUFFER_SIZE;
	client->state = ANONYMOUS;
	client->facing = rand() % 4;
	client->pos.x = rand() % ctrl->params.width;
	client->pos.y = rand() % ctrl->params.height;
	return (true);
}

bool add_new_client(control_t *ctrl)
{
	client_t *cl = calloc(1, sizeof(client_t));

	CHECK(cl, == 0, false);
	CHECK(cl->cmd = llist_init(), == 0, false);
	CHECK(cl->pending = llist_init(), == 0, false);
	cl->id = get_id();
	CHECK(fill_base_stats(ctrl, cl), == false, false);
	CHECK(fill_ip(ctrl, cl), == false, false);
	CHECK(cl->node = poll_add(&ctrl->list, cl->fd, POLLIN | POLLOUT), == 0,
		false);
	add_pending(cl, strdup(WELCOME_MSG));
	CHECK(llist_push(ctrl->clients, 1, cl), == -1, false);
	return (true);
}
