/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** clear
*/

#include "list.h"

void llist_clear(list_t *this, bool free_payload)
{
	void *payload;

	if (this == 0 || this->head == 0)
		return;
	while (llist_size(this)) {
		payload = llist_pop(this);
		if (free_payload)
			free(payload);
	}
}
