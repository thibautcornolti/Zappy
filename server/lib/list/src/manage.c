/*
** EPITECH PROJECT, 2018
** test
** File description:
** test
*/

#include "list.h"
#include <stdarg.h>

ssize_t llist_push(list_t *this, size_t nb_args, ...)
{
	list_elem_t *head;
	va_list list;

	if (this == NULL)
		return (-1);
	va_start(list, nb_args);
	for (size_t i = 0; i < nb_args; i++) {
		head = calloc(1, sizeof(list_elem_t));
		if (head == 0)
			return (-1);
		head->payload = va_arg(list, void *);
		head->prev = this->last;
		this->head = this->head ? this->head : head;
		if (this->last)
			this->last->next = head;
		this->last = head;
		this->length += 1;
	}
	va_end(list);
	return (this->length);
}

void *llist_pop(list_t *this)
{
	list_elem_t *head;
	list_elem_t *last;
	void *payload = 0;

	if (this == NULL)
		return (0);
	head = this->last;
	last = this->last ? this->last->prev : 0;
	if (head) {
		payload = head->payload;
		free(head);
	}
	if (last)
		last->next = 0;
	else
		this->head = 0;
	this->last = last;
	this->length -= 1;
	return (payload);
}

void *llist_remove(list_t *this, size_t idx)
{
	list_elem_t **ptr;
	list_elem_t *head;
	void *payload;
	size_t i = 0;

	if (this == NULL || this->head == NULL)
		return (0);
	for (head = this->head; head && i++ != idx; head = head->next);
	if (head == 0)
		return (0);
	ptr = head->prev ? &head->prev->next : &this->head;
	*ptr = head->next;
	ptr = head->next ? &head->next->prev : &this->last;
	*ptr = head->prev;
	payload = head->payload;
	this->length -= 1;
	free(head);
	return (payload);
}

void *llist_reduce(list_t *this,
	void *(*func)(void *context, void *acc, void *elem, size_t idx),
	void *context, void *acc)
{
	size_t count = 0;

	if (this == 0)
		return (acc);
	for (list_elem_t *head = this->head; head; head = head->next)
		acc = func(context, acc, head->payload, count++);
	return (acc);
}
