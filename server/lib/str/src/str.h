/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** str
*/

#ifndef LIBSTR_H_
#define LIBSTR_H_

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum e_strtype { LSTR_STR, LSTR_CHAR, LSTR_INT } strtype_t;

char *lstr_append(char *, char *);
char *lstr_prepend(char *, char *);
char *lstr_concat(char *, size_t, ...);
char *lstr_reverse(char *);
char *lstr_capitalize(char *);
char *lstr_to_upper_case(char *);
char *lstr_to_lower_case(char *);
char *lstr_substr(char *, size_t, size_t);
char **lstr_split(const char *, char *);
char *lstr_slice(char *, size_t, size_t);
char *lstr_replace(char *, char *, char *);
char *lstr_trim(char *);
char *lstr_epur(char *);
ssize_t lstr_index_of(char *, size_t, char *);
bool lstr_equals(char *, char *);
bool lstr_match(char *, char *);
bool lstr_test(char *, char *);
bool lstr_includes(char *, char *);
ssize_t lstr_nb_occur(char *, char *);
bool lstr_ends_with(char *, char *);
bool lstr_starts_with(char *, char *);

#endif /* !LIBSTR_H_ */
