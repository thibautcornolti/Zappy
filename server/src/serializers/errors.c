/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** errors
*/

#include "server.h"

object_t *emit_command_error(void)
{
	object_t *ret = lobj_init();

	lobj_set(ret, "type", "string", strdup("error"));
	lobj_set(ret, "error-type", "string", strdup("command-error"));
	return (ret);
}

object_t *emit_syntax_error(void)
{
	object_t *ret = lobj_init();

	lobj_set(ret, "type", "string", strdup("error"));
	lobj_set(ret, "error-type", "string", strdup("syntax-error"));
	return (ret);
}
