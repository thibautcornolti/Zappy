/*
** EPITECH PROJECT, 2021
** PSU_zappy_2017
** File description:
** Created by rectoria
*/

#include "server.h"

bool init_map(control_t *ctrl)
{
	size_t height = ctrl->params.height;
	size_t width = ctrl->params.width;

	CHECK(ctrl->map = calloc(height * width, sizeof(cell_t)), == 0, false);
	for (size_t i = 0; ctrl->map && i < height * width; ++i) {
		ctrl->map[i].y = i / width;
		ctrl->map[i].x = i % width;
		CHECK(ctrl->map[i].items = llist_init(), == 0, false);
	}
	return (true);
}

list_t *map_get(control_t *control, size_t x, size_t y)
{
	if (y >= control->params.height || x >= control->params.width)
		return (0);
	return (control->map[x + y * control->params.width].items);
}

void map_add(control_t *control, size_t x, size_t y, item_t item)
{
	if (y >= control->params.height || x >= control->params.width)
		return;
	llist_push(control->map[x + y * control->params.width].items, 1, item);
}

bool find_item(item_t *item, item_t elem, size_t idx)
{
	(void)(idx);
	return (*item == elem);
}

void map_remove(control_t *control, size_t x, size_t y, item_t item)
{
	ssize_t idx;
	list_t *items;

	if (y >= control->params.height || x >= control->params.width)
		return;
	items = control->map[x + y * control->params.width].items;
	idx = llist_find_index(
		items, (bool (*)(void *, void *, size_t))find_item, &item);
	if (idx == -1)
		return;
	llist_remove(items, idx);
}
