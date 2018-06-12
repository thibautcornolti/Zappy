/*
** EPITECH PROJECT, 2018
** test
** File description:
** test
*/

#include "list.h"

void *llist_at(list_t *this, size_t idx)
{
	list_elem_t *head;
	size_t i = 0;

	if (this == NULL || this->head == NULL || idx >= this->length)
		return (NULL);
	for (head = this->head; head && i++ != idx; head = head->next);
	return (head ? head->payload : 0);
}

void *llist_front(list_t *this)
{
	return ((this && this->head) ? this->head->payload : 0);
}

void *llist_back(list_t *this)
{
	return ((this && this->last) ? this->last->payload : 0);
}

void llist_destroy(list_t *this)
{
	list_elem_t *head = 0;
	list_elem_t *last = 0;

	if (this != NULL) {
		head = this->head;
		while (head) {
			free(last);
			last = head;
			head = head->next;
		}
		free(last);
	}
	free(this);
}

list_t *llist_init(void)
{
	list_t *this = calloc(1, sizeof(list_t));

	if (this == 0)
		return (NULL);
	this->head = NULL;
	this->last = NULL;
	this->length = 0;
	return (this);
}