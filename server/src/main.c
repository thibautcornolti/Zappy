/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** main
*/

#include "lpoll/src/common.h"
#include "lsocket/src/socket.h"

int main()
{
	poll_t *list = 0;
	int fd;
	
	CHECK(fd = create_server(4242), -1, 84);
	CHECK(poll_add(&list, fd, POLLIN), 0, 84);
	while (1) {
		CHECK(poll_wait(list, -1), -1, 84);
		dprintf(1, "EVENT !\n");
	}
	return 0;
}
