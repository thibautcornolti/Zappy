/*
** EPITECH PROJECT, 2018
** test
** File description:
** test
*/

#include "list.h"

ssize_t llist_index_of(list_t *this, void *elem)
{
	list_elem_t *head;
	size_t i = 0;

	if (this == NULL || this->head == NULL)
		return (-1);
	for (head = this->head; head && head->payload != elem;
		head = head->next)
		i++;
	return (head ? (ssize_t)i : (ssize_t)-1);
}

ssize_t llist_find_index(list_t *this,
	bool (*predicate)(void *context, void *elem, size_t idx),
	void *context)
{
	list_elem_t *head;
	size_t i = 0;

	if (this == NULL || this->head == NULL)
		return (-1);
	for (head = this->head;
		head && predicate(context, head->payload, i++) == false;
		head = head->next);
	return (head ? (ssize_t)(i - 1) : (ssize_t)-1);
}

ssize_t llist_last_index_of(list_t *this, void *elem)
{
	list_elem_t *head;
	size_t i = this->length - 1;

	if (this == NULL || this->head == NULL)
		return (-1);
	for (head = this->last; head && head->payload != elem;
		head = head->prev)
		i--;
	return (head ? (ssize_t)i : (ssize_t)-1);
}

ssize_t llist_find_last_index(list_t *this,
	bool (*predicate)(void *context, void *elem, size_t idx),
	void *context)
{
	list_elem_t *head;
	size_t i = this->length - 1;

	if (this == NULL || this->head == NULL)
		return (-1);
	for (head = this->last;
		head && predicate(context, head->payload, i--) == false;
		head = head->prev);
	return (head ? (ssize_t)(i + 1) : (ssize_t)-1);
}
