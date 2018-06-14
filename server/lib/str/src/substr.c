/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** substr
*/

#include "str.h"

/*
** Returns the string between the start and end indexes.
**
** Does not modify the original string.
*/
char *lstr_slice(char *this, size_t start, size_t end)
{
	char *ret;

	if (this == NULL)
		return (NULL);
	ret = NULL;
	if (start <= strlen(this))
		ret = strndup(this + start, end - start);
	return (ret);
}

/*
** Returns the string of a specified length from the start index.
**
** Does not modify the original string.
*/
char *lstr_substr(char *this, size_t start, size_t length)
{
	char *ret;

	if (this == NULL)
		return (NULL);
	ret = NULL;
	if (start <= strlen(this))
		ret = strndup(this + start, length);
	return (ret);
}

/*
** Returns this string reversed.
**
** Does not modify the original string.
*/
char *lstr_reverse(char *this)
{
	int len = 0;
	int idx = 0;
	char *ret = 0;

	if (this == NULL)
		return (NULL);
	len = strlen(this);
	ret = calloc(len + 1, sizeof(char));
	if (ret == NULL)
		return (NULL);
	while (idx < len) {
		idx += 1;
		ret[idx - 1] = this[len - idx];
	}
	ret[len] = 0;
	return (ret);
}

static int assign(int *var, int value)
{
	*var = value;
	return (*var);
}

char *lstr_replace(char *this, char *old, char *new)
{
	char *ret = 0;
	int idx = 0;
	int offs = 0;

	if (this == NULL || old == NULL || new == NULL)
		return (NULL);
	while (idx != -1) {
		if (assign(&idx, lstr_index_of(this, offs, old)) == -1)
			break;
		ret = calloc(strlen(this) - strlen(old) + strlen(new) + 1, 1);
		if (ret == NULL)
			return (NULL);
		ret = strncpy(ret, this, idx);
		ret[idx] = 0;
		ret = strcat(strcat(ret, new), this + idx + strlen(old));
		free(this);
		this = ret;
		offs = idx + strlen(new);
	}
	return (this);
}
