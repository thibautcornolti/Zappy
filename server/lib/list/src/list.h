/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** stack
*/

#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct list_elem_s {
	void *payload;
	struct list_elem_s *next;
	struct list_elem_s *prev;
} list_elem_t;

typedef struct list_s {
	list_elem_t *head;
	list_elem_t *last;
	size_t length;
} list_t;

list_t *llist_init(void);
size_t llist_size(list_t *this);
void *llist_at(list_t *this, size_t idx);
ssize_t llist_insert(list_t *this, size_t idx, void *elem);
ssize_t llist_push(list_t *this, size_t nb_args, ...);
void *llist_pop(list_t *this);
void *llist_front(list_t *this);
void *llist_back(list_t *this);
void *llist_remove(list_t *this, size_t idx);
ssize_t llist_index_of(list_t *this, void *elem);
ssize_t llist_find_index(list_t *this,
	bool (*predicate)(void *context, void *elem, size_t idx),
	void *context);
ssize_t llist_last_index_of(list_t *this, void *elem);
ssize_t llist_find_last_index(list_t *this,
	bool (*predicate)(void *context, void *elem, size_t idx),
	void *context);
bool llist_includes(list_t *this, void *elem);
void *llist_shift(list_t *this);
ssize_t llist_unshift(list_t *this, size_t nb_args, ...);
void *llist_find(list_t *this,
	bool (*predicate)(void *context, void *elem, size_t idx),
	void *context);
list_t *llist_filter(list_t *this,
	bool (*predicate)(void *context, void *elem, size_t idx),
	void *context);
list_t *llist_sort(list_t *this,
	int (*predicate)(void *a, size_t idx1, void *b, size_t idx2));
list_t *llist_reverse(list_t *this);
void llist_for_each(list_t *this,
	void (*func)(void *context, void *elem, size_t idx), void *context);
void *llist_reduce(list_t *this,
	void *(*func)(void *context, void *acc, void *elem, size_t idx),
	void *context, void *acc);
void llist_clear(list_t *this, bool free_payload);
void llist_destroy(list_t *this);
