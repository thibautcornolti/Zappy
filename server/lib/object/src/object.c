/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** object
*/

#include "object.h"

void lobj_for_each(object_t *this,
	void (*callback)(void *context, char *key, elem_t *elem),
	void *context)
{
	int idx = -1;
	char **keys = this->key_table;
	elem_t **values = this->value_table;

	if (this == NULL || callback == NULL)
		return;
	while (++idx < this->length)
		callback(context, keys[idx], values[idx]);
}

object_t *lobj_init(void)
{
	object_t *this = malloc(sizeof(object_t));

	if (this == NULL)
		return (NULL);
	this->key_table = NULL;
	this->value_table = NULL;
	this->length = 0;
	return (this);
}