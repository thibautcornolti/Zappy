/*
** EPITECH PROJECT, 2018
** PSU_zappy_2017
** File description:
** server.h
*/

#pragma once

#include <stddef.h>

#define RBUFFER_SIZE 4096
#define CMD_SIZE 2048

typedef struct rbuf_s {
	char buffer[RBUFFER_SIZE];
	int start;
	int end;
	int size;
} rbuf_t;

typedef struct cmd_s {
	rbuf_t rbuf;
	char cmd[CMD_SIZE];
	char name[CMD_SIZE];
	char **param;
	int nparam;
} cmd_t;

typedef struct params_s {
	size_t port;
	size_t width;
	size_t height;
	char **teams;
	size_t nclt;
	size_t tickrate;
} params_t;