/*
** EPITECH PROJECT, 2018
** PSU_zappy_2017
** File description:
** server.h
*/

#pragma once

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