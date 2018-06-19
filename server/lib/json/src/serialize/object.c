/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** object
*/

#include "../libjson.h"

static int internal_ljson_serialize_object_helper(
	char **output, int idx, char **keys, elem_t **values)
{
	if (idx) {
		*output = lstr_append(*output, ",");
		if (*output == NULL)
			return (-1);
	}
	else {
		*output = lstr_append(*output, "");
		if (*output == NULL)
			return (-1);
	}
	*output = lstr_concat(*output, 3, LSTR_STR, "\"", LSTR_STR, keys[idx],
		LSTR_STR, "\":");
	if (*output == NULL || ljson_serialize_elem(values[idx], output) == -1)
		return (-1);
	return (0);
}

int ljson_serialize_object(elem_t *elem, char **output)
{
	char **keys = ((object_t *)elem->data)->key_table;
	elem_t **values = ((object_t *)elem->data)->value_table;
	int size = ((object_t *)elem->data)->length;
	int idx = -1;

	*output = lstr_append(*output, "{");
	if (*output == NULL)
		return (-1);
	while (++idx < size)
		if (internal_ljson_serialize_object_helper(
			output, idx, keys, values) == -1)
			return (-1);
	*output = lstr_append(*output, "}");
	if (*output == NULL)
		return (-1);
	return (0);
}

static int internal_ljson_serialize_list_helper(
	char **output, int idx, elem_t *elem)
{
	if (idx) {
		*output = lstr_append(*output, ",");
		if (*output == NULL)
			return (-1);
	}
	if (ljson_serialize_elem(llist_at(elem->data, idx), output) == -1)
		return (-1);
	return (0);
}

int ljson_serialize_list(elem_t *elem, char **output)
{
	int idx = -1;
	int size = llist_size(elem->data);

	*output = lstr_append(*output, "[");
	if (*output == NULL)
		return (-1);
	while (++idx < size)
		if (internal_ljson_serialize_list_helper(output, idx, elem) ==
			-1)
			return (-1);
	*output = lstr_append(*output, "]");
	if (*output == NULL)
		return (-1);
	return (0);
}