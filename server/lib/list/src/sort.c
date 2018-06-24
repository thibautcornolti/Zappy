/*
** EPITECH PROJECT, 2018
** test
** File description:
** test
*/

#include "list.h"

void internal_llist_swap_nodes(list_t *this, list_elem_t *head)
{
	list_elem_t *next;
	list_elem_t **ptr = head->prev ? &head->prev->next : &this->head;

	*ptr = head->next;
	ptr = head->next->next ? &head->next->next->prev : &this->last;
	*ptr = head;
	head->next->prev = head->prev;
	head->prev = head->next;
	next = head->next->next;
	head->next->next = head;
	head->next = next;
}

static bool llist_sort_helper(
	list_t *this, int (*predicate)(void *, size_t, void *, size_t))
{
	bool done = true;
	size_t i = 0;

	for (list_elem_t *head = this->head; head && head->next;
		head = head->next) {
		if (predicate(head->payload, i, head->next->payload, i) > 0) {
			done = false;
			internal_llist_swap_nodes(this, head);
		}
		i += 1;
	}
	return (done);
}

list_t *llist_sort(
	list_t *this, int (*predicate)(void *, size_t, void *, size_t))
{
	if (this == 0 || this->head == 0)
		return (0);
	while (llist_sort_helper(this, predicate) == false);
	return (this);
}

ssize_t llist_insert(list_t *this, size_t idx, void *elem)
{
	list_elem_t *head;
	list_elem_t *ref;
	list_elem_t **ptr;
	size_t i = 0;

	if (this == NULL)
		return (-1);
	for (ref = this->head; ref && i++ != idx; ref = ref->next);
	head = calloc(1, sizeof(list_elem_t));
	if (head == 0)
		return (-1);
	head->payload = elem;
	head->next = ref;
	head->prev = ref ? ref->prev : this->last;
	ptr = ref ? &ref->prev : &this->last;
	*ptr = head;
	ptr = head->prev ? &head->prev->next : &this->head;
	*ptr = head;
	this->length += 1;
	return (this->length);
}

size_t llist_size(list_t *this)
{
	if (this == NULL)
		return (0);
	return (this->length);
}
