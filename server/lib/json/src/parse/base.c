/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** base
*/

#include "../libjson.h"
#include "../json.h"

int ljson_parse_elem(char *raw, int idx, object_t *root)
{
	char *key;
	char *type;
	void *data;

	idx = ljson_parse_str(raw, idx, (void **)&key);
	if (idx == -1 || lobj_get(root, key) != NULL || raw[idx++] != ':')
		return (-1);
	type = ljson_find_type(raw, idx);
	if (type == NULL)
		return (-1);
	idx = ljson_assign_value(raw, idx, type, &data);
	if (idx == -1 || lobj_set(root, key, type, data) == NULL)
		return (-1);
	free(key);
	return (idx);
}

elem_t *ljson_init_root(char *raw, int idx)
{
	char *type = ljson_find_type(raw, idx);
	void *data;

	if (type == NULL || ljson_assign_value(raw, idx, type, &data) == -1)
		return (NULL);
	return (lobj_elem_init(type, data));
}

elem_t *ljson_build_tree(char *raw)
{
	if (raw == NULL || (raw[0] != '{' && raw[0] != '['))
		return (NULL);
	return (ljson_init_root(raw, 0));
}