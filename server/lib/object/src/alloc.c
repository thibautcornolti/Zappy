/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** alloc
*/

#include "object.h"

elem_t *lobj_get(object_t *this, char *key)
{
	int idx;
	char *elem;

	if (this == NULL || !this->length || key == NULL)
		return (NULL);
	idx = -1;
	while (++idx < this->length) {
		elem = this->key_table[idx];
		if (lstr_equals(elem, key))
			return (this->value_table[idx]);
	}
	return (NULL);
}

static void internal_lobj_add_helper(
	object_t *this, char **nkeys, elem_t **nvals)
{
	int idx = -1;

	while (++idx < this->length) {
		nkeys[idx] = this->key_table[idx];
		nvals[idx] = this->value_table[idx];
	}
}

static elem_t *internal_lobj_add(
	object_t *this, char *key, char *type, void *data)
{
	elem_t **nvals = malloc(sizeof(elem_t *) * (this->length + 1));
	char **nkeys = malloc(sizeof(char *) * (this->length + 1));
	elem_t *elem = malloc(sizeof(*elem));

	if (elem == NULL || nvals == NULL || nkeys == NULL)
		return (NULL);
	internal_lobj_add_helper(this, nkeys, nvals);
	nkeys[this->length] = strdup(key);
	elem->type = strdup(type);
	if (nkeys[this->length] == NULL || elem->type == NULL)
		return (NULL);
	elem->data = data;
	nvals[this->length++] = elem;
	free(this->key_table);
	this->key_table = nkeys;
	free(this->value_table);
	this->value_table = nvals;
	return (elem);
}

elem_t *lobj_set(object_t *this, char *key, char *type, void *data)
{
	elem_t *elem;

	if (this == NULL || key == NULL)
		return (NULL);
	elem = lobj_get(this, key);
	if (elem != NULL) {
		elem->data = data;
		elem->type = strdup(type);
		if (elem->type == NULL)
			return (NULL);
		return (elem);
	}
	else
		return (internal_lobj_add(this, key, type, data));
}
