/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** init
*/

#include "server.h"

bool ctrl_init(control_t *ctrl)
{
	CHECK(ctrl->clients = llist_init(), == 0, false);
	CHECK(ctrl->eggs = llist_init(), == 0, false);
	CHECK(init_map(ctrl), == false, false);
	CHECK(place_resources(ctrl), == false, false);
	CHECK(team_init(ctrl), == false, false);
	return (true);
}
