/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

void team_realloc_arr(control_t *ctrl)
{
	params_t *p = &ctrl->params;

	p->nteam -= 1;
	ctrl->teams = realloc(ctrl->teams, sizeof(team_t) * p->nteam);
	p->teams = realloc(p->teams, sizeof(char *) * p->nteam);
}

void team_release(team_t *team)
{
	free(team->name);
	free(team->cl);
}