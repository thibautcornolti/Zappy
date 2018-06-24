/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** capitalize
*/

#include "str.h"

static int is_maj(const char c)
{
	return (c >= 'A' && c <= 'Z');
}

static int is_min(const char c)
{
	return (c >= 'a' && c <= 'z');
}

static int is_alphanum(const char c)
{
	return (is_min(c) || is_maj(c) || (c >= '0' && c <= '9'));
}

/*
** Capitalizes every words of a string.
** Words are separated by non-alphanumeric characters.
**
** Does directly modify the original string.
*/
char *lstr_capitalize(char *this)
{
	int i;

	if (this == NULL)
		return (NULL);
	i = -1;
	while (this[++i])
		if (is_min(this[i]) &&
			(i == 0 || (i && !is_alphanum(this[i - 1]))))
			this[i] += ('A' - 'a');
		else if (i && is_maj(this[i]) && is_alphanum(this[i - 1]))
			this[i] += ('a' - 'A');
	return (this);
}
