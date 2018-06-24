/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** transform
*/

#include "str.h"

static int internal_lstr_spaces_len(const char *str)
{
	int count = -1;

	while (str[++count])
		if (str[count] != ' ' && str[count] != '\t')
			return (count);
	return (count);
}

char *lstr_epur(char *this)
{
	char *tmp;
	int start = -1;
	int len;

	if (this == NULL)
		return (NULL);
	while (this[++start])
		if (this[start] == ' ' || this[start] == '\t') {
			len = internal_lstr_spaces_len(this + start);
			tmp = calloc(strlen(this) - len + 2, sizeof(char));
			if (tmp == NULL)
				return (NULL);
			tmp = strncpy(tmp, this, start);
			tmp[start] = 0;
			strcat(strcat(tmp, " "), this + start + len);
			free(this);
			this = tmp;
		}
	return (this);
}

/*
** Does not modify the original string
*/
char *lstr_trim(char *this)
{
	int start = -1;
	int end;

	if (this == NULL)
		return (NULL);
	while (this[++start] && (this[start] == ' ' || this[start] == '\t'));
	end = strlen(this) - 1;
	while (end >= 0 &&
		(this[end] == ' ' || this[end] == '\t' || this[end] == '\n'))
		end -= 1;
	return (lstr_slice(this, start, end + 1));
}

/*
** Transforms this string to uppercase.
**
** Does directly modify the original string.
*/
char *lstr_to_upper_case(char *this)
{
	int idx = -1;

	if (this == NULL)
		return (NULL);
	while (this[++idx])
		this[idx] = toupper(this[idx]);
	return (this);
}

/*
** Transforms this string to lowercase.
**
** Does directly modify the original string.
*/
char *lstr_to_lower_case(char *this)
{
	int idx = -1;

	if (this == NULL)
		return (NULL);
	while (this[++idx])
		this[idx] = tolower(this[idx]);
	return (this);
}
