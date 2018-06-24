/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

void event_tickrate_switchover(control_t *ctrl)
{
	object_t *ret = lobj_init();
	object_t *data = lobj_init();

	lobj_set(ret, "type", "string", strdup("event"));
	lobj_set(ret, "event-type", "string", strdup("tickrate-switchover"));
	lobj_set(data, "tickrate", "number",
		double_to_void_ptr(ctrl->params.tickrate));
	lobj_set(ret, "data", "object", data);
	forward_event(ctrl, ret);
}