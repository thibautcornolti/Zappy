/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

void init_map(control_t *ctrl)
{
	size_t height = ctrl->params.height;
	size_t width = ctrl->params.width;

	ctrl->map = calloc(height * width, sizeof(cell_t));
	for (size_t i = 0; ctrl->map && i < height * width; ++i) {
		ctrl->map[i].y = i / width;
		ctrl->map[i].x = i % width;
		ctrl->map[i].item = llist_init();
	}
}