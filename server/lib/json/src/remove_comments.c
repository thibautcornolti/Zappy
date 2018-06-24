/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** remove_comments
*/

#include "libjson.h"

char *ljson_remove_comment(char *str, int idx, int type)
{
	char *tab[2][3] = {{"//", "\n", NULL}, {"/*", "*/", NULL}};
	int tabidx;
	int len = strlen(tab[type][0]);
	int adv = idx + len - 1;

	while (str[++adv]) {
		tabidx = 0;
		while (tab[type][++tabidx])
			COMMENT_REPLACE
		len += 1;
	}
	return (ljson_remove_substr(str, idx, len));
}
