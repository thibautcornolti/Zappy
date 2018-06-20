/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

bool team_add_client(control_t *ctrl, client_t *cl, char *team)
{
	bool ret = false;
	team_t *t;

	for (size_t i = 0; i < ctrl->params.nteam; ++i) {
		t = &ctrl->teams[i];
		if (!strcmp(t->name, team) && t->av) {
			t->cl[ctrl->params.nclt - t->av - 1] = cl;
			cl->team = t;
			t->av -= 1;
			ret = true;
		}
	}
	return (ret);
}

bool team_remove_client(control_t *ctrl, client_t *cl)
{
	size_t i = 0;
	bool ret = false;

	if (cl->team) {
		for (; i < ctrl->params.nclt; ++i)
			if (!ret && cl->team->cl[i] == cl) {
				cl->team->cl[i] = 0;
				ret = true;
			}
			else if (ret && i < ctrl->params.nclt - 1) {
				cl->team->cl[i] = cl->team->cl[i + 1];
				cl->team->cl[i + 1] = 0;
			}
		cl->team = (ret) ? 0 : cl->team;
	}
	return (ret);
}

void team_add(control_t *ctrl, char *str)
{
	params_t *p = &ctrl->params;

	p->nteam += 1;
	ctrl->teams = realloc(ctrl->teams, sizeof(team_t) * p->nteam);
	p->teams = realloc(p->teams, sizeof(char *) * p->nteam);
	p->teams[p->nteam - 1] = strdup(str);
	ctrl->teams[p->nteam - 1].name = p->teams[p->nteam - 1];
	ctrl->teams[p->nteam - 1].cl = calloc(p->nteam, sizeof(team_t));
	ctrl->teams[p->nteam - 1].av = p->nclt;
}

bool team_remove(control_t *ctrl, char *name)
{
	bool ret = false;

	for (size_t i = 0; i < ctrl->params.nteam; ++i) {
		if (!ret && !strcmp(ctrl->teams[i].name, name)) {
			for (size_t k = 0;
				k < ctrl->params.nclt - ctrl->teams[i].av; ++k)
				ctrl->teams[i].cl[k]->team = 0;
			team_release(&ctrl->teams[i]);
			i -= 1;
			ret = true;
		}
		else if (ret && i < ctrl->params.nteam - 1) {
			ctrl->params.teams[i] = ctrl->params.teams[i + 1];
			ctrl->teams[i].name = ctrl->params.teams[i];
			ctrl->teams[i].cl = ctrl->teams[i + 1].cl;
			ctrl->teams[i].av = ctrl->teams[i + 1].av;
		}
	}
	if (ret)
		team_realloc_arr(ctrl);
	return (ret);
}

bool team_init(control_t *ctrl)
{
	CHECK(ctrl->teams = calloc(ctrl->params.nteam, sizeof(team_t)), == 0,
		false);
	for (size_t i = 0; i < ctrl->params.nteam; ++i) {
		ctrl->teams[i].name = ctrl->params.teams[i];
		ctrl->teams[i].av = ctrl->params.nclt;
		CHECK(ctrl->teams[i].cl =
				calloc(ctrl->teams[i].av, sizeof(client_t *)),
			== 0, false);
	}
	return (true);
}