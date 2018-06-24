/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** serialize_position
*/

#include "server.h"

object_t *serialize_position(vec2_t pos)
{
	object_t *ret = lobj_init();

	lobj_set(ret, "x", "number", double_to_void_ptr(pos.x));
	lobj_set(ret, "y", "number", double_to_void_ptr(pos.y));
	return (ret);
}
