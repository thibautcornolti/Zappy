/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** object
*/

#include "object.h"

void lobj_for_each(object_t *this, void (*callback)(char *key, elem_t *elem))
{
	int idx;

	if (this == NULL || callback == NULL)
		return;
	idx = -1;
	while (++idx < this->length)
		callback(this->key_table[idx], this->value_table[idx]);
}

object_t *init_object(void)
{
	object_t *this = malloc(sizeof(object_t));

	if (this == NULL)
		return (NULL);
	this->key_table = NULL;
	this->value_table = NULL;
	this->length = 0;
	return (this);
}