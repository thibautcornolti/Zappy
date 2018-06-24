/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

void gui_tickrate(control_t *ctrl, client_t *cl)
{
	object_t *ret = lobj_init();

	lobj_set(ret, "type", "string", strdup("response"));
	lobj_set(ret, "response-type", "string", strdup("tickrate"));
	lobj_set(ret, "tickrate", "number", double_to_void_ptr(ctrl->params.tickrate));
	finalize_json(cl, ret, cl->cmd->head->payload);
}