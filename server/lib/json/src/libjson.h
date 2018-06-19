/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** libjson
*/

#pragma once

#include "object/src/object.h"
#include "list/src/list.h"

#define COMMENT_REPLACE                                                       \
	if (lstr_starts_with(str + adv, tab[type][tabidx])) {                 \
		len += strlen(tab[type][tabidx]);                             \
		return (ljson_remove_substr(str, idx, len));                  \
	}

typedef struct json_parse_func_s {
	char *type;
	int (*func)(char *, int, void **);
} json_parse_func_t;

typedef struct json_serialize_func_s {
	char *type;
	int (*func)(elem_t *, char **);
} json_serialize_func_t;

/*
** Preprocessing
*/
char *ljson_normalize(char *str);
bool ljson_is_space(char c);
int ljson_is_comment(char *str, int idx);
char *ljson_remove_comment(char *str, int idx, int type);
char *ljson_pack(char *str);
char *ljson_clear(char *str);
int ljson_space_len(char *str, int idx);
char *ljson_remove_substr(char *str, int idx, int len);

/*
** Parsing
*/
elem_t *ljson_build_tree(char *raw);
char *ljson_find_type(char *raw, int idx);
int ljson_assign_value(char *raw, int idx, char *type, void **data);
int ljson_parse_elem(char *raw, int idx, object_t *root);
int ljson_parse_str(char *raw, int idx, void **data);
int ljson_parse_number(char *raw, int idx, void **data);
int ljson_parse_object(char *raw, int idx, void **data);
int ljson_parse_list(char *raw, int idx, void **data);
int ljson_parse_bool(char *raw, int idx, void **data);
int ljson_parse_null(char *raw, int idx, void **data);

/*
** Serializing
*/
int ljson_serialize_elem(elem_t *elem, char **output);
int ljson_serialize_str(elem_t *elem, char **output);
int ljson_serialize_number(elem_t *elem, char **output);
int ljson_serialize_object(elem_t *elem, char **output);
int ljson_serialize_list(elem_t *elem, char **output);
int ljson_serialize_bool(elem_t *elem, char **output);
int ljson_serialize_null(elem_t *elem, char **output);

