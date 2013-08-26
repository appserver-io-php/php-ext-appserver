/*
 * appserver_llist.c
 *
 *  Created on: 23.08.2013
 *      Author: zelgerj
 */

#include <stdlib.h>
#include <string.h>

#include "appserver_llist.h"

appserver_llist *appserver_llist_alloc(appserver_llist_dtor dtor)
{
	appserver_llist *l;

	l = malloc(sizeof(appserver_llist));
	appserver_llist_init(l, dtor);

	return l;
}

void appserver_llist_init(appserver_llist *l, appserver_llist_dtor dtor)
{
	l->size = 0;
	l->dtor = dtor;
	l->head = NULL;
	l->tail = NULL;
}

int appserver_llist_insert_next(appserver_llist *l, appserver_llist_element *e, const void *p)
{
	appserver_llist_element  *ne;

	if (!e) {
		e = APPSERVER_LLIST_TAIL(l);
	}

	ne = (appserver_llist_element *) malloc(sizeof(appserver_llist_element));
	ne->ptr = (void *) p;
	if (l->size == 0) {
		l->head = ne;
		l->head->prev = NULL;
		l->head->next = NULL;
		l->tail = ne;
	} else {
		ne->next = e->next;
		ne->prev = e;
		if (e->next) {
			e->next->prev = ne;
		} else {
			l->tail = ne;
		}
		e->next = ne;
	}

	++l->size;

	return 1;
}

int appserver_llist_insert_prev(appserver_llist *l, appserver_llist_element *e, const void *p)
{
	appserver_llist_element *ne;

	if (!e) {
		e = APPSERVER_LLIST_HEAD(l);
	}

	ne = (appserver_llist_element *) malloc(sizeof(appserver_llist_element));
	ne->ptr = (void *) p;
	if (l->size == 0) {
		l->head = ne;
		l->head->prev = NULL;
		l->head->next = NULL;
		l->tail = ne;
	} else {
		ne->next = e;
		ne->prev = e->prev;
		if (e->prev)
			e->prev->next = ne;
		else
			l->head = ne;
		e->prev = ne;
	}

	++l->size;

	return 0;
}

int appserver_llist_remove(appserver_llist *l, appserver_llist_element *e, void *user)
{
	if (e == NULL || l->size == 0)
		return 0;

	if (e == l->head) {
		l->head = e->next;

		if (l->head == NULL)
			l->tail = NULL;
		else
			e->next->prev = NULL;
	} else {
		e->prev->next = e->next;
		if (!e->next)
			l->tail = e->prev;
		else
			e->next->prev = e->prev;
	}

	if (l->dtor) {
		l->dtor(user, e->ptr);
	}
	free(e);
	--l->size;

	return 0;
}

int appserver_llist_remove_next(appserver_llist *l, appserver_llist_element *e, void *user)
{
	return appserver_llist_remove(l, e->next, user);
}

int appserver_llist_remove_prev(appserver_llist *l, appserver_llist_element *e, void *user)
{
	return appserver_llist_remove(l, e->prev, user);
}

appserver_llist_element *appserver_llist_jump(appserver_llist *l, int where, int pos)
{
	appserver_llist_element *e=NULL;
    int i;

    if (where == LIST_HEAD) {
        e = APPSERVER_LLIST_HEAD(l);
        for (i = 0; i < pos; ++i) {
            e = APPSERVER_LLIST_NEXT(e);
        }
    }
    else if (where == LIST_TAIL) {
        e = APPSERVER_LLIST_TAIL(l);
        for (i = 0; i < pos; ++i) {
            e = APPSERVER_LLIST_PREV(e);
        }
    }

    return e;
}

size_t appserver_llist_count(appserver_llist *l)
{
	return l->size;
}

void appserver_llist_empty(appserver_llist *l, void *user)
{
	while (appserver_llist_count(l) > 0) {
		appserver_llist_remove(l, APPSERVER_LLIST_TAIL(l), user);
	}
}

void appserver_llist_destroy(appserver_llist *l, void *user)
{
	appserver_llist_empty(l, user);

	free (l);
}

/*
 * Local Variables:
 * c-basic-offset: 4
 * tab-width: 4
 * End:
 * vim600: fdm=marker
 * vim: noet sw=4 ts=4
 */
