/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

static tuple_t fcmd[] = {
		{"msv", &cmd_msv}, {"bct", &cmd_bct}, {"mct", &cmd_mct},
		{"tna", &cmd_tna}, {"ppo", &cmd_ppo}, {"plv", &cmd_plv},
		{"pin", &cmd_pin}, {"sgt", &cmd_sgt}, {"sst", &cmd_sst}
};