/*
** EPITECH PROJECT, 2018
** zappy
** File description:
** resource_gen
*/

#include "item_names.h"

bool place_resource(control_t *control, size_t x, size_t y)
{
	list_t *items;

	if (rand() % 2)
		return (true);
	items = control->map[x + y * control->params.width].items;
	if (rand() % 2)
		llist_push(items, 1, FOOD);
	else
		llist_push(items, 1, rand() % 6 + 1);
	return (true);
}

// char *get_names(control_t *control, char *acc, item_t elem, size_t idx)
// {
// 	size_t size = strlen(item_names[elem]) + strlen(acc) + 1 + !!idx;
// 	char *ret = malloc(sizeof(char) * size);

// 	(void)(idx);
// 	(void)(control);
// 	strcpy(ret, acc);
// 	if (idx)
// 		strcat(ret, " ");
// 	strcpy(ret, item_names[elem]);
// 	free(acc);
// 	return (ret);
// }

bool place_resources(control_t *control)
{
	for (size_t y = 0; y < control->params.height; ++y)
		for (size_t x = 0; x < control->params.width; ++x)
			CHECK(place_resource(control, x, y), == false, false);
	// for (size_t y = 0; y < control->params.height; ++y) {
	// 	printf("|");
	// 	for (size_t x = 0; x < control->params.width; ++x) {
	// 		list_t *l = map_get(control, x, y);
	// 		char *names = llist_reduce(l,
	// 			(void *(*)(void *, void *, void *,
	// 				size_t))get_names,
	// 			control, strdup(""));
	// 		printf(x ? "|%s" : "%s", names);
	// 		free(names);
	// 	}
	// 	printf("|\n");
	// }
}
