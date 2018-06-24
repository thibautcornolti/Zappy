/*
** EPITECH PROJECT, 2018
** test
** File description:
** test
*/

#include "list.h"
#include <stdarg.h>

void *llist_shift(list_t *this)
{
	if (this == NULL)
		return (NULL);
	return (llist_remove(this, 0));
}

ssize_t llist_unshift(list_t *this, size_t nb_args, ...)
{
	va_list list;
	size_t count;

	if (this == NULL)
		return (-1);
	va_start(list, nb_args);
	count = -1;
	while (++count < nb_args)
		if (llist_insert(this, 0, va_arg(list, void *)) == -1)
			return (-1);
	return (this->length);
}

list_t *llist_reverse(list_t *this)
{
	list_elem_t *tmp;

	if (this == NULL)
		return (NULL);
	for (list_elem_t *head = this->last; head; head = head->next) {
		tmp = head->next;
		head->next = head->prev;
		head->prev = tmp;
	}
	tmp = this->head;
	this->head = this->last;
	this->last = tmp;
	return (this);
}

void llist_for_each(list_t *this,
	void (*func)(void *context, void *elem, size_t idx), void *context)
{
	size_t count = 0;

	if (this == NULL || this->head == NULL)
		return;
	for (list_elem_t *head = this->head; head; head = head->next)
		func(context, head->payload, count++);
}
