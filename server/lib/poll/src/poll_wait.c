/*
** EPITECH PROJECT, 2018
** Project
** File description:
** poll_wait.c
*/

#include <glob.h>
#include <poll.h>
#include "common.h"

static size_t size_list(poll_t *p)
{
	size_t ret = 0;

	while (p) {
		ret += 1;
		p = p->next;
	}
	return (ret);
}

static void poll_apply(struct pollfd *p_to, poll_t *p_src)
{
	size_t i = 0;

	while (p_src) {
		p_to[i].fd = p_src->fd;
		p_to[i].events = p_src->evt;
		p_to[i].revents = 0;
		i += 1;
		p_src = p_src->next;
	}
}

static void poll_apply_end(struct pollfd *p_to, poll_t *p_src)
{
	size_t i = 0;

	while (p_src) {
		p_src->revt = p_to[i].revents;
		i += 1;
		p_src = p_src->next;
	}
}

int poll_wait(poll_t *p, int timeout)
{
	int ret;
	size_t size = size_list(p);
	struct pollfd p_to[size];

	poll_apply(p_to, p);
	ret = poll(p_to, size, timeout);
	poll_apply_end(p_to, p);
	return (ret);
}