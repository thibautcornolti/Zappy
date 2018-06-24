/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** split
*/

#include "str.h"

static int internal_lstr_len_to_substr(const char *src, const char *suffix)
{
	char *idx;

	if (src == NULL || suffix == NULL)
		return (-1);
	idx = strstr(src, suffix);
	if (idx == NULL)
		return (-1);
	return (idx - src);
}

static char **internal_lstr_add_tab(char **tab, char *to_add)
{
	char **ret;
	int len = (tab == NULL) - 1;

	while (tab && tab[++len]);
	ret = calloc(len + 2, sizeof(char *));
	if (ret == NULL)
		return (NULL);
	len = (tab == NULL) - 1;
	while (tab && tab[++len])
		ret[len] = tab[len];
	ret[len++] = to_add;
	ret[len] = NULL;
	free(tab);
	return (ret);
}

static char **intern_lstr_split_helper(
	const char *this, const char *sep, char **ret, int *idx)
{
	int len = internal_lstr_len_to_substr(this + *idx, sep);
	char *to_add =
	((len == -1) ? strdup(this + *idx) : strndup(this + *idx, len));

	if (to_add == NULL)
		return (0);
	*idx += strlen(to_add) + ((len == -1) ? 0 : strlen(sep)) - 1;
	ret = internal_lstr_add_tab(ret, to_add);
	if (ret == NULL)
		return (0);
	return (ret);
}

/*
** Splits the string by the specified separator string.
**
** Does not modify the original string.
*/
char **lstr_split(const char *this, const char *sep)
{
	char **ret = 0;
	int idx = -1;

	if (this == NULL || sep == NULL)
		return (NULL);
	while (this[++idx]) {
		ret = intern_lstr_split_helper(this, sep, ret, &idx);
		if (ret == 0)
			return (0);
	}
	return (ret);
}
