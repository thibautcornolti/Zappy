/*
** EPITECH PROJECT, 2018
** test
** File description:
** test
*/

#include "list.h"

void *llist_find(list_t *this,
	bool (*predicate)(void *context, void *elem, size_t idx),
	void *context)
{
	list_elem_t *head;
	size_t i = 0;

	if (this == NULL || this->head == NULL)
		return (0);
	for (head = this->head;
		head && predicate(context, head->payload, i++) == false;
		head = head->next);
	return (head ? head->payload : 0);
}

list_t *llist_filter(list_t *this,
	bool (*predicate)(void *context, void *elem, size_t idx),
	void *context)
{
	list_elem_t *head;
	list_t *out;
	size_t i = 0;

	if (this == NULL || this->head == NULL)
		return (0);
	out = llist_init();
	for (head = this->head; head; head = head->next)
		if (predicate(context, head->payload, i++))
			llist_push(out, 1, head->payload);
	return (out);
}

bool llist_includes(list_t *this, void *elem)
{
	return (this ? (llist_index_of(this, elem) != -1) : false);
}
