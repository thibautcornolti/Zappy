/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** is
*/

#include "libjson.h"

bool ljson_is_space(char c)
{
	return ((c == ' ') || (c == '\n') || (c == '\t'));
}

int ljson_is_comment(char *str, int idx)
{
	if (strncmp(str + idx, "//", 2) == 0)
		return (0);
	if (strncmp(str + idx, "/*", 2) == 0)
		return (1);
	return (-1);
}
