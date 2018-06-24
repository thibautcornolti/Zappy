/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** clear
*/

#include "libjson.h"

static bool handle_escape(char *str, int idx, char *escape)
{
	char comment_type;

	comment_type = ljson_is_comment(str, idx);
	if (str[idx] == '"' || str[idx] == '\'')
		*escape = str[idx];
	else if (comment_type != -1) {
		str = ljson_remove_comment(str, idx--, comment_type);
		if (str == NULL)
			return (false);
	}
	return (true);
}

char *ljson_clear(char *str)
{
	int idx = -1;
	char escape = 0;

	while (str[++idx])
		if (escape && escape == str[idx])
			escape = 0;
		else if (!escape && handle_escape(str, idx, &escape) == false)
			return (0);
	return (str);
}
