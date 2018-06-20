/*
** EPITECH PROJECT, 2018
** PSU_myftp_2017
** File description:
** object
*/

#ifndef LIBOBJECT_H_
#define LIBOBJECT_H_

#include "str/src/str.h"

typedef struct elem_s {
	char *type;
	void *data;
} elem_t;

typedef struct object_s {
	int length;
	char **key_table;
	elem_t **value_table;
} object_t;

object_t *lobj_init(void);
elem_t *lobj_elem_init(char *type, void *data);
void lobj_free_elem(elem_t *elem, bool free_payload);
elem_t *lobj_get(object_t *this, char *key);
elem_t *lobj_set(object_t *this, char *key, char *type, void *data);
void lobj_remove(object_t *this, char *key);
char **lobj_keys(object_t *this);
elem_t **lobj_values(object_t *this);
void lobj_destroy(object_t *this);
void lobj_for_each(object_t *this, void (*callback)(char *key, elem_t *elem));

#endif /* !LIBOBJECT_H_ */
