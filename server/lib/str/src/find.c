/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** find
*/

#include "str.h"

/*
** Returns whether the specified string is found within this string.
*/
bool lstr_includes(char *this, char *to_find)
{
	if (this == NULL)
		return (false);
	return (lstr_index_of(this, 0, to_find) != -1);
}

/*
** Returns the number of times the specified string
** is found within this string
*/
ssize_t lstr_nb_occur(char *this, char *str)
{
	int nb_occur;
	int idx;

	if (this == NULL || str == NULL)
		return (0);
	nb_occur = 0;
	idx = 0;
	while ((idx = lstr_index_of(this, idx, str)) != -1) {
		idx += 1;
		nb_occur += 1;
	}
	return (nb_occur);
}

/*
** Returns whether this string starts with the specified string.
*/
bool lstr_starts_with(char *this, char *prefix)
{
	if (this == NULL)
		return (false);
	return (lstr_index_of(this, 0, prefix) == 0);
}

/*
** Returns whether this string ends with the specified string.
*/
bool lstr_ends_with(char *this, char *suffix)
{
	int cur;
	int len;

	if (this == NULL || suffix == NULL)
		return (false);
	cur = strlen(this);
	len = strlen(suffix);
	if (cur < len)
		return (false);
	cur -= len;
	return (strcmp(this + cur, suffix) == 0);
}

/*
** Returns the index of the first occurence of the specified string
** within this string starting at an offset.
*/
ssize_t lstr_index_of(char *this, size_t offset, char *to_find)
{
	char *idx;

	if (this == NULL || to_find == NULL)
		return (-1);
	if (offset > strlen(this))
		return (-1);
	idx = strstr(this + offset, to_find);
	if (idx == NULL)
		return (-1);
	return ((int)(idx - this));
}
