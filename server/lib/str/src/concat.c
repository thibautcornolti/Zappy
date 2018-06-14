/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** concat
*/

#include "str.h"

/*
** Prepends the specifed string to this string.
**
** Does directly modify the original string.
*/
char *lstr_prepend(char *this, char *str)
{
	char *ret;

	if (this == NULL || str == NULL)
		return (NULL);
	ret = calloc(strlen(this) + strlen(str) + 1, sizeof(char));
	if (ret == NULL)
		return (NULL);
	ret = strcat(strcpy(ret, str), this);
	free(this);
	return (ret);
}

/*
** Appends the specifed string to this string.
**
** Does directly modify the original string.
*/
char *lstr_append(char *this, char *str)
{
	char *ret;

	if (this == NULL || str == NULL)
		return (NULL);
	ret = calloc(strlen(this) + strlen(str) + 1, sizeof(char));
	if (ret == NULL)
		return (NULL);
	ret = strcat(strcpy(ret, this), str);
	free(this);
	return (ret);
}

static char *int_to_str(const int nb)
{
	char *ret = 0;
	int len = !nb;
	int i = nb;

	while (i) {
		len += 1;
		i /= 10;
	}
	ret = calloc(len + 1, sizeof(char));
	if (ret == NULL)
		return (NULL);
	i = nb;
	ret[len] = 0;
	while (i) {
		ret[--len] = (i % 10) + '0';
		i /= 10;
	}
	if (nb == 0)
		ret[0] = '0';
	return (ret);
}

static char *get_str(const strtype_t arg, va_list *list, bool *to_free)
{
	char *to_use;

	to_use = NULL;
	*to_free = false;
	if (arg == LSTR_STR)
		to_use = va_arg(*list, char *);
	else if (arg == LSTR_CHAR) {
		to_use = strdup("a");
		if (to_use == NULL)
			return (NULL);
		to_use[0] = va_arg(*list, int);
		*to_free = true;
	}
	else if (arg == LSTR_INT) {
		to_use = int_to_str(va_arg(*list, int));
		*to_free = true;
	}
	return (to_use);
}

/*
** Concatenates this string with one or more strings.
**
** Usage:
**   You must specify the number of strings to concatenate.
**   For each string to concatenate, you must specify:
**     - The type of the string (STDSTR for 'char *', STR_t for 'char *').
**     - The string itself.
**
** Exemple:
**   str->concat(str, 2, STDSTR, "foo", STR_t, init_str(NULL, "bar"));
**
** Does directly modify the original string.
*/
char *lstr_concat(char *this, size_t nb, ...)
{
	va_list list;
	strtype_t arg;
	bool to_free = false;
	char *to_use;

	if (this == NULL)
		return (NULL);
	if (nb <= 0)
		return (this);
	va_start(list, nb);
	while (nb > 0) {
		nb -= 1;
		arg = va_arg(list, strtype_t);
		to_use = get_str(arg, &list, &to_free);
		this = (to_use != NULL) ? lstr_append(this, to_use) : this;
		if (to_free)
			free(to_use);
	}
	va_end(list);
	return (this);
}
