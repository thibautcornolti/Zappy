/*
** EPITECH PROJECT, 2018
** Project
** File description:
** common.h
*/

#pragma once

#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct poll_s {
	int fd;
	short evt;
	short revt;
	struct poll_s *next;
} poll_t;

int poll_add(poll_t **p, int fd, short evt);
int poll_rm(poll_t **p, int fd);
int poll_update(poll_t *p, int fd, short evt);

int poll_wait(poll_t *p, int timeout);

int poll_canread(poll_t *p, int fd);
int poll_canwrite(poll_t *p, int fd);
int poll_isclose(poll_t *p, int fd);