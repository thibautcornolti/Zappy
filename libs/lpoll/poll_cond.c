/*
** EPITECH PROJECT, 2018
** Project
** File description:
** poll_cond.c
*/

#include <poll.h>
#include "common.h"

int poll_canread(poll_t *p, int fd)
{
	while (p) {
		if (p->fd == fd && (POLLIN & p->revt) != 0)
			return (1);
		else if (p->fd == fd)
			return (0);
		p = p->next;
	}
	return (0);
}

int poll_canwrite(poll_t *p, int fd)
{
	while (p) {
		if (p->fd == fd && (POLLOUT & p->revt) != 0)
			return (1);
		else if (p->fd == fd)
			return (0);
		p = p->next;
	}
	return (0);
}

int poll_isclose(poll_t *p, int fd)
{
	while (p) {
		if (p->fd == fd && (POLLHUP & p->revt) != 0)
			return (1);
		else if (p->fd == fd)
			return (0);
		p = p->next;
	}
	return (1);
}