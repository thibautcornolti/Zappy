/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** json
*/

#include "json.h"
#include "libjson.h"

json_t *ljson_parse(char *text)
{
	json_t *json;

	if (text == NULL)
		return (NULL);
	json = malloc(sizeof(json_t));
	if (json == NULL)
		return (NULL);
	json->raw = strdup(text);
	if (json->raw == NULL)
		return (NULL);
	json->original = strdup(text);
	json->raw = ljson_normalize(json->raw);
	json->root = ljson_build_tree(json->raw);
	if (json->root == NULL)
		return (NULL);
	return (json);
}

char *ljson_serialize(elem_t *root)
{
	char *output;

	if (root == NULL)
		return (NULL);
	output = strdup("");
	if (output == NULL || ljson_serialize_elem(root, &output) == -1)
		return (output);
	if (output == NULL)
		return (NULL);
	return (output);
}