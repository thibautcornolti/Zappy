/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** pack
*/

#include "libjson.h"

static bool handle_escape(char **str, int *idx, char *escape)
{
	int len;

	if ((*str)[(*idx)] == '"' || (*str)[(*idx)] == '\'')
		*escape = (*str)[(*idx)];
	else if (ljson_is_space((*str)[(*idx)])) {
		len = ljson_space_len((*str), (*idx));
		(*str) = ljson_remove_substr((*str), (*idx)--, len);
		if ((*str) == 0)
			return (false);
	}
	return (true);
}

char *ljson_pack(char *str)
{
	int idx = -1;
	char escape = 0;

	while (str[++idx])
		if (escape && escape == str[idx])
			escape = 0;
		else if (!escape) {
			if (handle_escape(&str, &idx, &escape) == false)
				return (0);
		}
	if (escape)
		return (0);
	return (str);
}
