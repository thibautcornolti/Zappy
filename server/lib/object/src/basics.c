/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** basics
*/

#include "object.h"

static void intern_lobj_delete_helper(
	object_t *this, char *key, char **nkeys, elem_t **nvals)
{
	int idx1 = 0;
	int idx2 = 0;
	while (idx1 < this->length) {
		if (!lstr_equals(this->key_table[idx1], key)) {
			nkeys[idx2] = this->key_table[idx1];
			nvals[idx2] = this->value_table[idx1];
			idx2++;
		}
		else {
			free(this->value_table[idx1]->type);
			free(this->value_table[idx1]);
			free(this->key_table[idx1]);
		}
		idx1++;
	}
}

void lobj_remove(object_t *this, char *key)
{
	elem_t **nvals;
	char **nkeys;

	if (this == NULL || key == NULL || lobj_get(this, key) == 0)
		return;
	nvals = malloc(sizeof(elem_t *) * (this->length - 1));
	if (nvals == NULL)
		return;
	nkeys = malloc(sizeof(char *) * (this->length - 1));
	if (nkeys == NULL)
		return;
	intern_lobj_delete_helper(this, key, nkeys, nvals);
	this->length -= 1;
	free(this->key_table);
	this->key_table = nkeys;
	free(this->value_table);
	this->value_table = nvals;
}

char **lobj_keys(object_t *this)
{
	if (this == NULL)
		return (NULL);
	return (this->key_table);
}

elem_t **lobj_values(object_t *this)
{
	if (this == NULL)
		return (NULL);
	return (this->value_table);
}

void lobj_destroy(object_t *this)
{
	if (this) {
		for (int i = 0; i < this->length; i++) {
			free(this->key_table[i]);
			free(this->value_table[i]->type);
			free(this->value_table[i]);
		}
		free(this->key_table);
		free(this->value_table);
	}
	free(this);
}