/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** base
*/

#include "../libjson.h"
#include <stdio.h>

int ljson_serialize_bool(elem_t *elem, char **output)
{
	char *arr[2] = {"false", "true"};

	*output = lstr_append(*output, arr[(long)elem->data]);
	if (*output == NULL)
		return (-1);
	return (0);
}

int ljson_serialize_null(elem_t *elem, char **output)
{
	(void)elem;
	*output = lstr_append(*output, "null");
	if (*output == NULL)
		return (-1);
	return (0);
}

int ljson_serialize_number(elem_t *elem, char **output)
{
	double value = (*((double *)&elem->data));
	int size = snprintf(0, 0, "%g", value);
	char *str = 0;

	if (size == -1)
		return (-1);
	str = malloc(sizeof(char) * (size + 1));
	if (str == 0 || snprintf(str, size + 1, "%g", value) == -1)
		return (-1);
	*output = lstr_append(*output, str);
	free(str);
	if (*output == NULL)
		return (-1);
	return (0);
}

int ljson_serialize_str(elem_t *elem, char **output)
{
	*output = lstr_concat(*output, 3, LSTR_STR, "\"", LSTR_STR, elem->data,
		LSTR_STR, "\"");
	if (*output == NULL)
		return (-1);
	return (0);
}

int ljson_serialize_elem(elem_t *elem, char **output)
{
	json_serialize_func_t branches[6] = {
		{"object", ljson_serialize_object},
		{"list", ljson_serialize_list},
		{"number", ljson_serialize_number},
		{"string", ljson_serialize_str},
		{"bool", ljson_serialize_bool},
		{"null", ljson_serialize_null}};
	int idx = -1;

	while (++idx < 6)
		if (lstr_equals(elem->type, branches[idx].type))
			return (branches[idx].func(elem, output));
	return (-1);
}
