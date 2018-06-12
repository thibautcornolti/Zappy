/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** socket
*/

#include "socket.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

int create_server(short port)
{
	struct sockaddr_in addr = {0};
	int fd;
	int opt = 1;

	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	CHECK(fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), != -1, -1);
	CHECK(setsockopt(fd, SOCK_STREAM, SO_REUSEADDR, &opt, sizeof(opt)),
		== -1, -1);
	CHECK(bind(fd, (const struct sockaddr *)&addr, sizeof(addr)), == -1,
		-1);
	CHECK(listen(fd, 1024), == -1, -1);
	return (fd);
}