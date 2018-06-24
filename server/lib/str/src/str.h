/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** str
*/

#pragma once

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum e_strtype { LSTR_STR, LSTR_CHAR, LSTR_INT } strtype_t;

char *lstr_append(char *, const char *);
char *lstr_prepend(char *, const char *);
char *lstr_concat(char *, size_t, ...);
char *lstr_reverse(char *);
char *lstr_capitalize(char *);
char *lstr_to_upper_case(char *);
char *lstr_to_lower_case(char *);
char *lstr_substr(const char *, size_t, size_t);
char **lstr_split(const char *, const char *);
char *lstr_slice(const char *, size_t, size_t);
char *lstr_replace(char *, const char *, const char *);
char *lstr_trim(char *);
char *lstr_epur(char *);
ssize_t lstr_index_of(const char *, size_t, const char *);
bool lstr_equals(const char *, const char *);
bool lstr_match(const char *, const char *);
bool lstr_test(const char *, const char *);
bool lstr_includes(const char *, const char *);
ssize_t lstr_nb_occur(const char *, const char *);
bool lstr_ends_with(const char *, const char *);
bool lstr_starts_with(const char *, const char *);
