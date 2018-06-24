/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** json
*/

#pragma once

#include "object/src/object.h"
#include "list/src/list.h"

typedef struct json_s {
	char *original;
	char *raw;
	elem_t *root;
} json_t;

json_t *ljson_parse(char *text);
char *ljson_serialize(elem_t *root);
