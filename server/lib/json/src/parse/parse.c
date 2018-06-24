/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** parse
*/

#include "../libjson.h"

int ljson_parse_str(char *raw, int idx, void **data)
{
	char quote[2] = {raw[idx], 0};
	int len;

	if (raw[idx] != '"' && raw[idx] != '\'')
		return (-1);
	idx += 1;
	len = lstr_index_of(raw, idx, quote);
	if (len == -1)
		return (-1);
	len -= idx;
	*((char **)data) = strndup(raw + idx, len);
	if (*((char **)data) == NULL)
		return (-1);
	idx += len + 1;
	return (idx);
}

int ljson_parse_number(char *raw, int idx, void **data)
{
	char *dest;

	*((double *)data) = strtod(raw + idx, &dest);
	if (*((double *)data) == 0 && dest == (raw + idx))
		return (-1);
	return (idx + dest - (raw + idx));
}

int ljson_parse_object(char *raw, int idx, void **data)
{
	bool hasNext;

	*((object_t **)data) = lobj_init();
	if (*((object_t **)data) == NULL)
		return (-1);
	idx += 1;
	hasNext = (raw[idx] && raw[idx] != '}');
	while (hasNext) {
		idx = ljson_parse_elem(raw, idx, *((object_t **)data));
		if (idx == -1)
			return (-1);
		hasNext = (raw[idx] == ',');
		idx += !!hasNext;
	}
	if (raw[idx++] != '}')
		return (-1);
	return (idx);
}

int ljson_parse_bool(char *raw, int idx, void **data)
{
	*((long *)data) = (raw[idx] == 't');
	return (idx + (4 + (raw[idx] == 'f')));
}

int ljson_parse_null(char *raw, int idx, void **data)
{
	(void)raw;
	*data = NULL;
	return (idx + 4);
}
