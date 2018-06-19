/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** list
*/

#include "../libjson.h"

int ljson_parse_list(char *raw, int idx, void **data)
{
	char *type;
	void *tmp_data;
	bool hasNext = (raw[idx + 1] && raw[idx + 1] != ']');

	*((list_t **)data) = llist_init();
	idx += 1;
	while (hasNext) {
		if (raw[idx] != ',') {
			type = ljson_find_type(raw, idx);
			if (type == NULL)
				return (-1);
			idx = ljson_assign_value(raw, idx, type, &tmp_data);
			if (idx == -1)
				return (-1);
		}
		llist_push(*((list_t **)data), 1, init_elem(type, tmp_data));
		hasNext = (raw[idx] == ',');
		idx += !!hasNext;
	}
	return ((raw[idx] != ']') ? -1 : (idx + 1));
}
