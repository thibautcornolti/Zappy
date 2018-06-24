/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

void helper_team_remove(control_t *control, size_t i)
{
	control->params.teams[i] = control->params.teams[i + 1];
	control->teams[i].name = control->params.teams[i];
	control->teams[i].cl = control->teams[i + 1].cl;
	control->teams[i].av = control->teams[i + 1].av;
}

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