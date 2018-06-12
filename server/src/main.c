/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** main
*/

#include "poll/src/common.h"
#include "socket/src/socket.h"
#include "list/src/list.h"

int main()
{
	poll_t *list = 0;
	int fd;

	CHECK(fd = create_server(4242), -1, 84);
	CHECK(poll_add(&list, fd, POLLIN), 0, 84);
	while (1) {
		CHECK(poll_wait(list, -1), -1, 84);
		if (poll_canread(list, fd)) {
		}
	}
	return 0;
}
