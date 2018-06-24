/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** types
*/

#include "../libjson.h"

static bool ljson_is_end_delim(char c)
{
	return (c == '}' || c == ']' || c == ',');
}

static bool ljson_is_bool(char *raw, int idx)
{
	if (lstr_starts_with(raw + idx, "true") &&
		ljson_is_end_delim(raw[idx + 4]))
		return (true);
	if (lstr_starts_with(raw + idx, "false") &&
		ljson_is_end_delim(raw[idx + 5]))
		return (true);
	return (false);
}

static bool ljson_is_null(char *raw, int idx)
{
	return (lstr_starts_with(raw + idx, "null") &&
		ljson_is_end_delim(raw[idx + 4]));
}

char *ljson_find_type(char *raw, int idx)
{
	char *tab[4][2] = {{"{", "object"}, {"[", "list"},
		{"-0123456789.", "number"}, {"\"'", "string"}};
	int tabidx = -1;
	int cidx = -1;
	int len;

	tabidx = -1;
	while (++tabidx < 4) {
		len = strlen(tab[tabidx][0]);
		cidx = -1;
		while (++cidx < len)
			if (tab[tabidx][0][cidx] == raw[idx])
				return (tab[tabidx][1]);
	}
	if (ljson_is_bool(raw, idx))
		return ("bool");
	else if (ljson_is_null(raw, idx))
		return ("null");
	return (NULL);
}

int ljson_assign_value(char *raw, int idx, char *type, void **data)
{
	int arr_idx = -1;
	json_parse_func_t func[6] = {{"object", ljson_parse_object},
		{"list", ljson_parse_list}, {"number", ljson_parse_number},
		{"string", ljson_parse_str}, {"bool", ljson_parse_bool},
		{"null", ljson_parse_null}};

	while (++arr_idx < 6)
		if (strcmp(func[arr_idx].type, type) == 0)
			return (func[arr_idx].func(raw, idx, data));
	return (-1);
}