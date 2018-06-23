/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** errors
*/

#include "server.h"

object_t *emit_command_error(char *reason)
{
	object_t *ret = lobj_init();

	lobj_set(ret, "type", "string", strdup("error"));
	lobj_set(ret, "error-type", "string", strdup("command-error"));
	if (reason)
		lobj_set(ret, "error-msg", "string", strdup(reason));
	return (ret);
}

object_t *emit_syntax_error(char *reason)
{
	object_t *ret = lobj_init();

	lobj_set(ret, "type", "string", strdup("error"));
	lobj_set(ret, "error-type", "string", strdup("syntax-error"));
	if (reason)
		lobj_set(ret, "error-msg", "string", strdup(reason));
	return (ret);
}
