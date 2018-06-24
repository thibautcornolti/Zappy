/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** trim
*/

#include "libjson.h"

int ljson_space_len(char *str, int idx)
{
	int len;

	len = 0;
	while (str[idx] && ljson_is_space(str[idx])) {
		idx += 1;
		len += 1;
	}
	return (len);
}

char *ljson_remove_substr(char *str, int idx, int len)
{
	char *ret = malloc(strlen(str) - len + 1);

	if (ret == NULL)
		return (NULL);
	strncpy(ret, str, idx);
	ret[idx] = 0;
	strcat(ret, str + idx + len);
	free(str);
	str = ret;
	return (str);
}

char *ljson_normalize(char *str)
{
	str = ljson_clear(str);
	if (str == NULL)
		return (NULL);
	str = ljson_pack(str);
	if (str == NULL)
		return (NULL);
	return (str);
}
