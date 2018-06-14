/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** compare
*/

#include "str.h"
#include "intern_str.h"

/*
** Returns whether the candidate string is equal to this string.
*/
bool lstr_equals(char *this, char *candidate)
{
	if (this == NULL || candidate == NULL)
		return (NULL);
	return (strcmp(this, candidate) == 0);
}

bool lstr_intern_match_helper(char *s1, char *s2, int idx1, int idx2)
{
	while (s1[idx1] != '\0') {
		if (lstr_intern_match(s1, s2, idx1, idx2 + 1))
			return (true);
		idx1 += 1;
	}
	return (lstr_intern_match(s1, s2, idx1, idx2 + 1));
}

bool lstr_intern_match(char *s1, char *s2, int idx1, int idx2)
{
	if (s2[idx2] == '\0')
		return (s1[idx1] == '\0');
	else if (s2[idx2] == '*')
		return (lstr_intern_match_helper(s1, s2, idx1, idx2));
	else if (s2[idx2] != '?' && s2[idx2] != s1[idx1])
		return (false);
	else
		return (lstr_intern_match(s1, s2, idx1 + 1, idx2 + 1));
}

/*
** Returns whether this string match with the specified pattern.
**
** Supports * and ? as wildcards but no escape mechanism.
*/
bool lstr_match(char *this, char *pattern)
{
	if (this == NULL || pattern == NULL)
		return (false);
	return (lstr_intern_match(this, pattern, 0, 0));
}

/*
** Tests whether the specified candidate match with the pattern described in
** this string.
**
** Supports * and ? as wildcards but no escape mechanism.
*/
bool lstr_test(char *this, char *candidate)
{
	if (this == NULL || candidate == NULL)
		return (false);
	return (lstr_intern_match(candidate, this, 0, 0));
}
